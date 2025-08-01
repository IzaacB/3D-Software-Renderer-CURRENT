#include "ctagss.h"
#include "image.h"
#include "raster.h"

static f32_array raster_lerp(f32 i0, f32 d0, f32 i1, f32 d1)
{
    f32_array line;
    u32 len = ceil(i1) - ceil(i0);
    array_init(f32, line, len);

    f32 slope = 0;

    if (i1 != i0)
    {
        slope = (d1 - d0) / (i1 - i0);
    }

    f32 d = d0 + slope * (ceil(i0) - i0);
    for (u32 i = 0; i < len; i++)
    {
        array_insert(line, d);
        d += slope;
    }

    return line;
}

void raster_ppx(i32 x, i32 y, color c)
{
    x = (precomp.half_canvas_width + x);
    y = (precomp.half_canvas_height - y);

    bool on_screen = x >= 0 && x < CANVAS_WIDTH && y >= 0 && y < CANVAS_HEIGHT;

    if (on_screen)
    {
        c.r = fmax(0, fmin(floor(c.r * settings.color_range_red), settings.color_range_red));
        c.g = fmax(0, fmin(floor(c.g * settings.color_range_green), settings.color_range_green));
        c.b = fmax(0, fmin(floor(c.b * settings.color_range_blue), settings.color_range_blue));

        f32 color_ratio_red = 255 / (f32)settings.color_range_red;
        f32 color_ratio_green = 255 / (f32)settings.color_range_green;
        f32 color_ratio_blue = 255 / (f32)settings.color_range_blue;

        u32 color_dword = (u8)(c.b * color_ratio_red) << 16 | (u8)(c.g * color_ratio_green) << 8 | (u8)(c.r * color_ratio_blue);

        state.surface[(u32)y * CANVAS_WIDTH + (u32)x] = color_dword;
    }
}

void raster_ppx_z(f32 x, f32 y, f32 z, color c)
{
    x = ceil(precomp.half_canvas_width + x);
    y = ceil(precomp.half_canvas_height - y);

    bool on_screen = x >= 0 && x < CANVAS_WIDTH && y >= 0 && y < CANVAS_HEIGHT;

    if (on_screen)
    {
        if (state.depth_buffer[(u32)y * CANVAS_WIDTH + (u32)x] > z)
        {
            c.r = fmax(0, fmin(floor(c.r * settings.color_range_red), settings.color_range_red));
            c.g = fmax(0, fmin(floor(c.g * settings.color_range_green), settings.color_range_green));
            c.b = fmax(0, fmin(floor(c.b * settings.color_range_blue), settings.color_range_blue));

            f32 color_ratio_red = 255 / (f32)settings.color_range_red;
            f32 color_ratio_green = 255 / (f32)settings.color_range_green;
            f32 color_ratio_blue = 255 / (f32)settings.color_range_blue;

            u32 color_dword = (u8)(c.b * color_ratio_red) << 16 | (u8)(c.g * color_ratio_green) << 8 | (u8)(c.r * color_ratio_blue);

            state.surface[(u32)y * CANVAS_WIDTH + (u32)x] = color_dword;
            state.depth_buffer[(u32)y * CANVAS_WIDTH + (u32)x] = z;
        }
    }
}

void raster_line(v2 p0, v2 p1, color c)
{
    if (p0.x == p1.x && p0.y == p1.y)
    {
        raster_ppx(p0.x, p0.y, c);
    }
    else if (fabsf(p1.x - p0.x) >= fabsf(p1.y - p0.y))
    {
        if (p1.x < p0.x)
        {
            v2 t = p0;
            p0 = p1;
            p1 = t;
        }

        f32_array line = raster_lerp(p0.x, p0.y, p1.x, p1.y);

        for (u32 x = 0; x < line.used; x++)
        {
            raster_ppx(x + p0.x, line.vals[x], c);
        }

        array_clear(line);
    }
    else
    {
        if (p1.y < p0.y)
        {
            v2 t = p0;
            p0 = p1;
            p1 = t;
        }

        f32_array line = raster_lerp(p0.y, p0.x, p1.y, p1.x);

        for (u32 y = 0; y < line.used; y++)
        {
            raster_ppx(line.vals[y], y + p0.y, c);
        }

        array_clear(line);
    }
}

void raster_triangle_wireframe(u32 i)
{
    v3 p0 = scene.projected.vals[scene.faces.vals[i].i0];
    v3 p1 = scene.projected.vals[scene.faces.vals[i].i1];
    v3 p2 = scene.projected.vals[scene.faces.vals[i].i2];

    v2 h0 = v3_to_v2(p0);
    v2 h1 = v3_to_v2(p1);
    v2 h2 = v3_to_v2(p2);

    color c = {1, 1, 1};
    raster_line(h0, h1, c);
    raster_line(h1, h2, c);
    raster_line(h2, h0, c);
}

void raster_triangle_solid(u32 i)
{
    v3 p0 = scene.projected.vals[scene.faces.vals[i].i0];
    v3 p1 = scene.projected.vals[scene.faces.vals[i].i1];
    v3 p2 = scene.projected.vals[scene.faces.vals[i].i2];

    v3 normal = scene.normals.vals[i];
    color base_color = scene.materials.vals[scene.material_indices.vals[i]].c;

    color directional_contribution = {0, 0, 0};

    for (u32 j = 0; j < scene.dir_lights.used; j++)
    {
        dir_light light = scene.dir_lights.vals[j];
        f32 dot = fmax(0, -v3_dot(normal, light.direction));

        directional_contribution.r += (dot * light.intensity * light.c.r);
        directional_contribution.g += (dot * light.intensity * light.c.g);
        directional_contribution.b += (dot * light.intensity * light.c.b);
    }

    color directional = {
        directional_contribution.r * base_color.r,
        directional_contribution.g * base_color.g,
        directional_contribution.b * base_color.b
    };

    color ambient = {
        base_color.r * settings.ambient_light.r,
        base_color.g * settings.ambient_light.g,
        base_color.b * settings.ambient_light.b
    };

    v3 t0;
    v2 t1;
    if (p1.y < p0.y)
    {
        t0 = p0;
        p0 = p1;
        p1 = t0;
    }

    if (p2.y < p0.y)
    {
        t0 = p0;
        p0 = p2;
        p2 = t0;
    }

    if (p2.y < p1.y)
    {
        t0 = p1;
        p1 = p2;
        p2 = t0;
    }

    f32 z0 = 1.0f / p0.z;
    f32 z1 = 1.0f / p1.z;
    f32 z2 = 1.0f / p2.z;

    f32_array x01 = raster_lerp(p0.y, p0.x, p1.y, p1.x);
    f32_array x12 = raster_lerp(p1.y, p1.x, p2.y, p2.x);
    f32_array x02 = raster_lerp(p0.y, p0.x, p2.y, p2.x);
    f32_array x012;
    array_concat(f32, x012, x01, x12);

    f32_array z01 = raster_lerp(p0.y, z0, p1.y, z1);
    f32_array z12 = raster_lerp(p1.y, z1, p2.y, z2);
    f32_array z02 = raster_lerp(p0.y, z0, p2.y, z2);
    f32_array z012;
    array_concat(f32, z012, z01, z12);

    f32_array x_left, x_right, z_left, z_right;

    if (x012.vals[(u32)(x012.used / 2)] < x02.vals[(u32)(x02.used / 2)])
    {
        x_left = x012;
        x_right = x02;

        z_left = z012;
        z_right = z02;

    }else
    {
        x_left = x02;
        x_right = x012;

        z_left = z02;
        z_right = z012;
    }

    for (u32 y_index = 0; y_index < x012.used; y_index++)
    {
        f32 y = y_index + ceil(p0.y);

        f32 x_start = x_left.vals[y_index];
        f32 x_end = x_right.vals[y_index];

        f32 z_start = (z_left.vals[y_index]);
        f32 z_end = (z_right.vals[y_index]);

        f32_array z_scan = raster_lerp(x_start, z_start, x_end, z_end);

        for (u32 x_index = 0; x_index < ceil(x_end) - ceil(x_start); x_index++)
        {
            f32 x = x_index + ceil(x_start);

            f32 z = (1.0f / z_scan.vals[x_index]);

            v3 deprojected = 
            {
                ((x / (CANVAS_WIDTH / viewport.t.scale.x)) * z) / viewport.t.scale.z,
                ((y / (CANVAS_HEIGHT / viewport.t.scale.y)) * z) / viewport.t.scale.z,
                z
            };

            color point = base_color;
            color point_contribution = {0, 0, 0};

            for (u32 i = 0; i < scene.point_lights.used; i++)
            {
                point_light light = scene.point_lights.vals[i];
                v3 light_to_pixel = v3_sub(light.position, deprojected);
                f32 distance = v3_mag(light_to_pixel);

                if (distance > light.range)
                {
                    continue;
                }

                f32 intensity = light.intensity / (distance * distance);
                f32 dot = fmax(0, v3_dot(normal, v3_norm(light_to_pixel)));

                point_contribution.r += (intensity * dot * light.c.r);
                point_contribution.g += (intensity * dot * light.c.g);
                point_contribution.b += (intensity * dot * light.c.b);
            }

            point.r *= point_contribution.r;
            point.g *= point_contribution.g;
            point.b *= point_contribution.b;

            color fully_lit = {
                ambient.r + directional.r + point.r,
                ambient.g + directional.g + point.g,
                ambient.b + directional.b + point.b
            };

            if (settings.fog)
            {
                f32 fog_affect = (z / (settings.render_distance * settings.render_distance)) * settings.fog_intensity;
                color fog = {
                    settings.fog_color.r * fog_affect,
                    settings.fog_color.g * fog_affect,
                    settings.fog_color.b * fog_affect
                };

                fully_lit.r -= fog.r;
                fully_lit.g -= fog.g;
                fully_lit.b -= fog.b;
            }
            
            raster_ppx_z(x, y, z, fully_lit);
        }
        array_clear(z_scan);
    }

    array_clear(x01);
    array_clear(x12);
    array_clear(x02);
    array_clear(x012);

    array_clear(z01);
    array_clear(z12);
    array_clear(z02);
    array_clear(z012);
}

void raster_triangle_textured(u32 i)
{
    v3 p0 = scene.projected.vals[scene.faces.vals[i].i0];
    v3 p1 = scene.projected.vals[scene.faces.vals[i].i1];
    v3 p2 = scene.projected.vals[scene.faces.vals[i].i2];

    v2 uv0 = scene.uvs.vals[scene.faces.vals[i].uv0];
    v2 uv1 = scene.uvs.vals[scene.faces.vals[i].uv1];
    v2 uv2 = scene.uvs.vals[scene.faces.vals[i].uv2];

    v3 normal = scene.normals.vals[i];
    image texture = scene.materials.vals[scene.material_indices.vals[i]].texture;

    color directional_contribution = {0, 0, 0};

    for (u32 j = 0; j < scene.dir_lights.used; j++)
    {
        dir_light light = scene.dir_lights.vals[j];
        f32 dot = fmax(0, -v3_dot(normal, light.direction));

        directional_contribution.r += (dot * light.intensity * light.c.r);
        directional_contribution.g += (dot * light.intensity * light.c.g);
        directional_contribution.b += (dot * light.intensity * light.c.b);
    }

    v3 t0;
    v2 t1;
    if (p1.y < p0.y)
    {
        t0 = p0;
        p0 = p1;
        p1 = t0;

        t1 = uv0;
        uv0 = uv1;
        uv1 = t1;
    }

    if (p2.y < p0.y)
    {
        t0 = p0;
        p0 = p2;
        p2 = t0;

        t1 = uv0;
        uv0 = uv2;
        uv2 = t1;
    }

    if (p2.y < p1.y)
    {
        t0 = p1;
        p1 = p2;
        p2 = t0;

        t1 = uv1;
        uv1 = uv2;
        uv2 = t1;
    }

    f32 z0 = 1.0f / p0.z;
    f32 z1 = 1.0f / p1.z;
    f32 z2 = 1.0f / p2.z;

    f32 u0 = uv0.x / p0.z;
    f32 u1 = uv1.x / p1.z;
    f32 u2 = uv2.x / p2.z;

    f32 v0 = uv0.y / p0.z;
    f32 v1 = uv1.y / p1.z;
    f32 v2 = uv2.y / p2.z;

    f32_array x01 = raster_lerp(p0.y, p0.x, p1.y, p1.x);
    f32_array x12 = raster_lerp(p1.y, p1.x, p2.y, p2.x);
    f32_array x02 = raster_lerp(p0.y, p0.x, p2.y, p2.x);
    f32_array x012;
    array_concat(f32, x012, x01, x12);

    f32_array z01 = raster_lerp(p0.y, z0, p1.y, z1);
    f32_array z12 = raster_lerp(p1.y, z1, p2.y, z2);
    f32_array z02 = raster_lerp(p0.y, z0, p2.y, z2);
    f32_array z012;
    array_concat(f32, z012, z01, z12);

    f32_array u01 = raster_lerp(p0.y, u0, p1.y, u1);
    f32_array u12 = raster_lerp(p1.y, u1, p2.y, u2);
    f32_array u02 = raster_lerp(p0.y, u0, p2.y, u2);
    f32_array u012;
    array_concat(f32, u012, u01, u12);

    f32_array v01 = raster_lerp(p0.y, v0, p1.y, v1);
    f32_array v12 = raster_lerp(p1.y, v1, p2.y, v2);
    f32_array v02 = raster_lerp(p0.y, v0, p2.y, v2);
    f32_array v012;
    array_concat(f32, v012, v01, v12);

    f32_array x_left, x_right, z_left, z_right, u_left, u_right, v_left, v_right;

    if (x012.vals[(u32)(x012.used / 2)] < x02.vals[(u32)(x02.used / 2)])
    {
        x_left = x012;
        x_right = x02;

        z_left = z012;
        z_right = z02;

        u_left = u012;
        u_right = u02;

        v_left = v012;
        v_right = v02;
    }else
    {
        x_left = x02;
        x_right = x012;

        z_left = z02;
        z_right = z012;

        u_left = u02;
        u_right = u012;

        v_left = v02;
        v_right = v012;
    }

    for (u32 y_index = 0; y_index < x012.used; y_index++)
    {
        f32 y = y_index + ceil(p0.y);

        f32 x_start = x_left.vals[y_index];
        f32 x_end = x_right.vals[y_index];

        f32 z_start = (z_left.vals[y_index]);
        f32 z_end = (z_right.vals[y_index]);

        f32 u_start = (u_left.vals[y_index]);
        f32 u_end = (u_right.vals[y_index]);

        f32 v_start = (v_left.vals[y_index]);
        f32 v_end = (v_right.vals[y_index]);

        f32_array z_scan = raster_lerp(x_start, z_start, x_end, z_end);
        f32_array u_scan = raster_lerp(x_start, u_start, x_end, u_end);
        f32_array v_scan = raster_lerp(x_start, v_start, x_end, v_end);

        for (u32 x_index = 0; x_index < ceil(x_end) - ceil(x_start); x_index++)
        {
            f32 x = x_index + ceil(x_start);

            f32 z = (1.0f / z_scan.vals[x_index]);

            f32 u = (u_scan.vals[x_index]) * z;
            f32 v = (v_scan.vals[x_index]) * z;

            i32 u_sample = (i32)round(u * texture.width) % texture.width;
            i32 v_sample = (i32)round(v * texture.height) % texture.height;

            color texel = image_sample(texture, u_sample, v_sample);

            color ambient = {
                texel.r * settings.ambient_light.r,
                texel.g * settings.ambient_light.g,
                texel.b * settings.ambient_light.b
            };

            color directional = {
                texel.r * directional_contribution.r,
                texel.g * directional_contribution.g,
                texel.b * directional_contribution.b
            };

             v3 deprojected = 
            {
                ((x / (CANVAS_WIDTH / viewport.t.scale.x)) * z) / viewport.t.scale.z,
                ((y / (CANVAS_HEIGHT / viewport.t.scale.y)) * z) / viewport.t.scale.z,
                z
            };

            color point = texel;
            color point_contribution = {0, 0, 0};

            for (u32 i = 0; i < scene.point_lights.used; i++)
            {
                point_light light = scene.point_lights.vals[i];
                v3 light_to_pixel = v3_sub(light.position, deprojected);
                f32 distance = v3_mag(light_to_pixel);

                if (distance > light.range)
                {
                    continue;
                }

                f32 intensity = light.intensity / (distance * distance);
                f32 dot = fmax(0, v3_dot(normal, v3_norm(light_to_pixel)));

                point_contribution.r += (intensity * dot * light.c.r);
                point_contribution.g += (intensity * dot * light.c.g);
                point_contribution.b += (intensity * dot * light.c.b);
            }

            point.r *= point_contribution.r;
            point.g *= point_contribution.g;
            point.b *= point_contribution.b;

            color fully_lit = {
                ambient.r + directional.r + point.r,
                ambient.g + directional.g + point.g,
                ambient.b + directional.b + point.b
            };

            if (settings.fog)
            {
                f32 fog_affect = (z / (settings.render_distance * settings.render_distance)) * settings.fog_intensity;
                color fog = {
                    settings.fog_color.r * fog_affect,
                    settings.fog_color.g * fog_affect,
                    settings.fog_color.b * fog_affect
                };

                fully_lit.r -= fog.r;
                fully_lit.g -= fog.g;
                fully_lit.b -= fog.b;
            }
            
            raster_ppx_z(x, y, z, fully_lit);
        }
        array_clear(z_scan);
        array_clear(u_scan);
        array_clear(v_scan);
    }

    array_clear(x01);
    array_clear(x12);
    array_clear(x02);
    array_clear(x012);

    array_clear(z01);
    array_clear(z12);
    array_clear(z02);
    array_clear(z012);

    array_clear(u01);
    array_clear(u12);
    array_clear(u02);
    array_clear(u012);

    array_clear(v01);
    array_clear(v12);
    array_clear(v02);
    array_clear(v012);
}
#include "ctagss.h"
#include "image.h"
#include "raster.h"

static f32_array raster_lerp(f32 i0, f32 d0, f32 i1, f32 d1)
{
    f32_array line;
    array_init(f32, line, CANVAS_WIDTH);

    f32 slope = 0;

    if (i1 != i0)
    {
        slope = (d1 - d0) / (i1 - i0);
    }

    for (u32 i = 0; i < (u32)(i1 - i0); i++)
    {
        array_insert(line, d0 + slope * i);
    }

    return line;
}

void raster_ppx(f32 x, f32 y, color c)
{
    x = ((f32)CANVAS_WIDTH / 2 + x);
    y = ((f32)CANVAS_HEIGHT / 2 - y);

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
    x = ((f32)CANVAS_WIDTH / 2 + x);
    y = ((f32)CANVAS_HEIGHT / 2 - y);

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
    color c = scene.materials.vals[scene.material_indices.vals[i]].c;
    color lit = {settings.ambient_light.r * c.r, settings.ambient_light.g * c.g, settings.ambient_light.b * c.b};

    for (u32 j = 0; j < scene.dir_lights.used; j++)
    {
        dir_light light = scene.dir_lights.vals[j];
        v3 light_dir = v3_norm(v3_transform(light.direction, viewport.t, 3));
        f32 dot = fmax(0, -v3_dot(normal, light_dir));

        color contribution =
            {
                dot * light.intensity * c.r * light.c.r,
                dot * light.intensity * c.g * light.c.g,
                dot * light.intensity * c.b * light.c.b};

        lit.r = lit.r + contribution.r;
        lit.g = lit.g + contribution.g;
        lit.b = lit.b + contribution.b;
    }

    v3 t;
    if (p1.y < p0.y)
    {
        t = p0;
        p0 = p1;
        p1 = t;
    }

    if (p2.y < p0.y)
    {
        t = p0;
        p0 = p2;
        p2 = t;
    }

    if (p2.y < p1.y)
    {
        t = p1;
        p1 = p2;
        p2 = t;
    }

    f32_array x01 = raster_lerp(p0.y, p0.x, p1.y, p1.x);
    f32_array x12 = raster_lerp(p1.y, p1.x, p2.y, p2.x);
    f32_array x02 = raster_lerp(p0.y, p0.x, p2.y, p2.x);
    f32_array x012;
    array_concat(f32, x012, x01, x12);

    f32_array z01 = raster_lerp(p0.y, 1 / p0.z, p1.y, 1 / p1.z);
    f32_array z12 = raster_lerp(p1.y, 1 / p1.z, p2.y, 1 / p2.z);
    f32_array z02 = raster_lerp(p0.y, 1 / p0.z, p2.y, 1 / p2.z);
    f32_array z012;
    array_concat(f32, z012, z01, z12);

    for (u32 y_index = 0; y_index < x012.used; y_index++)
    {
        i32 y = y_index + p0.y;

        f32 x_start = (x02.vals[y_index]);
        f32 x_end = (x012.vals[y_index]);

        f32 z_start = (z02.vals[y_index]);
        f32 z_end = (z012.vals[y_index]);

        if (x_start > x_end)
        {
            f32 temp = x_start;
            x_start = x_end;
            x_end = temp;

            temp = z_start;
            z_start = z_end;
            z_end = temp;
        }

        f32_array z_scan = raster_lerp(x_start, z_start, x_end + 1, z_end);

        for (i32 x = x_start; x < x_end; x++)
        {
            f32 z = 1 / z_scan.vals[(u32)(x - x_start)];
            if (settings.fog)
            {
                color lit_with_fog = lit;
                f32 fog_affect = (z / settings.render_distance) * settings.fog_intensity;

                lit_with_fog.r -= fog_affect * settings.fog_color.r;
                lit_with_fog.g -= fog_affect * settings.fog_color.g;
                lit_with_fog.b -= fog_affect * settings.fog_color.b;

                raster_ppx_z(x, y, z, lit_with_fog);
            }
            else
            {
                raster_ppx_z(x, y, z, lit);
            }
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
    color contribution = settings.ambient_light;

    for (u32 j = 0; j < scene.dir_lights.used; j++)
    {
        dir_light light = scene.dir_lights.vals[j];
        v3 light_dir = v3_norm(v3_transform(light.direction, viewport.t, 3));
        f32 dot = fmax(0, -v3_dot(normal, light_dir));

        contribution.r += (dot * light.intensity * light.c.r);
        contribution.g += (dot * light.intensity * light.c.g);
        contribution.b += (dot * light.intensity * light.c.b);
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

    f32_array x01 = raster_lerp(p0.y, p0.x, p1.y, p1.x);
    f32_array x12 = raster_lerp(p1.y, p1.x, p2.y, p2.x);
    f32_array x02 = raster_lerp(p0.y, p0.x, p2.y, p2.x);
    f32_array x012;
    array_concat(f32, x012, x01, x12);

    f32_array z01 = raster_lerp(p0.y, 1 / p0.z, p1.y, 1 / p1.z);
    f32_array z12 = raster_lerp(p1.y, 1 / p1.z, p2.y, 1 / p2.z);
    f32_array z02 = raster_lerp(p0.y, 1 / p0.z, p2.y, 1 / p2.z);
    f32_array z012;
    array_concat(f32, z012, z01, z12);

    f32_array u01 = raster_lerp(p0.y, uv0.x / p0.z, p1.y, uv1.x / p1.z);
    f32_array u12 = raster_lerp(p1.y, uv1.x / p1.z, p2.y, uv2.x / p2.z);
    f32_array u02 = raster_lerp(p0.y, uv0.x / p0.z, p2.y, uv2.x / p2.z);
    f32_array u012;
    array_concat(f32, u012, u01, u12);

    f32_array v01 = raster_lerp(p0.y, uv0.y / p0.z, p1.y, uv1.y / p1.z);
    f32_array v12 = raster_lerp(p1.y, uv1.y / p1.z, p2.y, uv2.y / p2.z);
    f32_array v02 = raster_lerp(p0.y, uv0.y / p0.z, p2.y, uv2.y / p2.z);
    f32_array v012;
    array_concat(f32, v012, v01, v12);

    for (u32 y_index = 0; y_index < x012.used; y_index++)
    {
        i32 y = y_index + p0.y;

        f32 x_start = (x02.vals[y_index]);
        f32 x_end = (x012.vals[y_index]);

        f32 z_start = (z02.vals[y_index]);
        f32 z_end = (z012.vals[y_index]);

        f32 u_start = (u02.vals[y_index]);
        f32 u_end = (u012.vals[y_index]);

        f32 v_start = (v02.vals[y_index]);
        f32 v_end = (v012.vals[y_index]);

        if (x_start > x_end)
        {
            f32 temp = x_start;
            x_start = x_end;
            x_end = temp;

            temp = z_start;
            z_start = z_end;
            z_end = temp;

            temp = u_start;
            u_start = u_end;
            u_end = temp;

            temp = v_start;
            v_start = v_end;
            v_end = temp;
        }

        f32_array z_scan = raster_lerp(x_start, z_start, x_end + 1, z_end);
        f32_array u_scan = raster_lerp(x_start, u_start, x_end + 1, u_end);
        f32_array v_scan = raster_lerp(x_start, v_start, x_end + 1, v_end);

        for (i32 x = round(x_start); x < round(x_end); x++)
        {
            i32 x_index = x - x_start;

            f32 z = 1 / z_scan.vals[x_index];

            f32 u = u_scan.vals[x_index] * z;
            f32 v = v_scan.vals[x_index] * z;

            i32 u_sample = (i32)round(u * texture.width) % texture.width;
            i32 v_sample = (i32)round(v * texture.height) % texture.height;

            color lit_texel = image_sample(texture, u_sample, v_sample);

            lit_texel.r *= contribution.r * lit_texel.r;
            lit_texel.g *= contribution.g * lit_texel.g;
            lit_texel.b *= contribution.b * lit_texel.b;

            if (settings.fog)
            {
                f32 fog_affect = (z / settings.render_distance) * settings.fog_intensity;
                lit_texel.r -= fog_affect * settings.fog_color.r;
                lit_texel.g -= fog_affect * settings.fog_color.g;
                lit_texel.b -= fog_affect * settings.fog_color.b;
            }

            raster_ppx_z(x, y, z, lit_texel);
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

void raster_sprite3D(image sprite, v3 pos, f32 size)
{
    v3 origin = v3_transform(pos, viewport.t, 1);

    if (origin.z > viewport.t.scale.z)
    {
        v3 point = {
            round((origin.x / origin.z * viewport.t.scale.z) * (CANVAS_WIDTH / viewport.t.scale.x)),
            round((origin.y / origin.z * viewport.t.scale.z) * (CANVAS_HEIGHT / viewport.t.scale.y)),
            origin.z};

        f32 screen_ratio = CANVAS_HEIGHT / sprite.width;
        f32 scale = fmax(.01, (size / point.z * viewport.t.scale.z) * screen_ratio);

        f32 scaled_w = sprite.width * scale;
        f32 scaled_h = sprite.height * scale;

        f32 fog_affect = (point.z / settings.render_distance) * settings.fog_intensity;

        v3 normal = {0, 0, -1};
        color contribution = settings.ambient_light;

        for (u32 j = 0; j < scene.dir_lights.used; j++)
        {
            dir_light light = scene.dir_lights.vals[j];
            v3 light_dir = v3_norm(v3_transform(light.direction, viewport.t, 3));
            f32 dot = fmax(0, -v3_dot(normal, light_dir));

            contribution.r += (dot * light.intensity * light.c.r);
            contribution.g += (dot * light.intensity * light.c.g);
            contribution.b += (dot * light.intensity * light.c.b);
        }

        for(i32 y = 0; y < floor(scaled_h); y++)
        {
            i32 sample_y = floor((f32)y / scale);

            for (i32 x = 0; x < floor(scaled_w); x++)
            {
                i32 sample_x = floor((f32)x / scale);

                color sample = image_sample(sprite, sample_x, sample_y);

                sample.r -= fog_affect * settings.fog_color.r;
                sample.g -= fog_affect * settings.fog_color.g;
                sample.b -= fog_affect * settings.fog_color.b;
                

                if (sample.a == 1)
                {
                    raster_ppx_z(point.x + x - scaled_w / 2, point.y - y + scaled_h / 2, point.z, sample);
                }
            }
        }
    }
}
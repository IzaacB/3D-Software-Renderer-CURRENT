#include "ctagss.h"
#include "scene.h"
#include "raster.h"

void scene_init()
{
    array_init(v3, scene.vertices);
    array_init(face, scene.faces);
    array_init(v3, scene.normals);
    array_init(material, scene.materials);
    array_init(u32, scene.material_indices);

    for (u32 i = 0; i < CANVAS_WIDTH * CANVAS_HEIGHT; i++)
    {
        state.depth_buffer[i] = RENDER_DISTANCE;
    }
}

void scene_clear()
{
    array_clear(scene.vertices);
    array_clear(scene.faces);
    array_clear(scene.normals);
    array_clear(scene.materials);
    array_clear(scene.material_indices);
}

static void scene_clip_plane(plane p)
{
    v3_array vertices_clip;
    array_init(v3, vertices_clip);

    face_array faces_clip;
    array_init(face, faces_clip);

    v3_array normals_clip;
    array_init(v3, normals_clip);

    u32_array material_indices_clip;
    array_init(u32, material_indices_clip);

    for (u32 i = 0; i < scene.faces.used; i++)
    {
        v3 p0 = scene.vertices.vals[scene.faces.vals[i].i0];
        v3 p1 = scene.vertices.vals[scene.faces.vals[i].i1];
        v3 p2 = scene.vertices.vals[scene.faces.vals[i].i2];

        v3 normal = scene.normals.vals[i];

        u32 material_index = scene.material_indices.vals[i];

        f32 d0 = v3_dot(p.normal, p0) + p.distance;
        f32 d1 = v3_dot(p.normal, p1) + p.distance;
        f32 d2 = v3_dot(p.normal, p2) + p.distance;

        bool p0_in = d0 >= 0;
        bool p1_in = d1 >= 0;
        bool p2_in = d2 >= 0;

        u32 new_vertex_indices[4];
        u8 num_clipped_vertices = 0;

        if (p0_in && p1_in && p2_in)
        {
            new_vertex_indices[0] = vertices_clip.used;
            array_insert(vertices_clip, p0);

            new_vertex_indices[1] = vertices_clip.used;
            array_insert(vertices_clip, p1);

            new_vertex_indices[2] = vertices_clip.used;
            array_insert(vertices_clip, p2);

            face f = 
            {
                new_vertex_indices[0],
                new_vertex_indices[1],
                new_vertex_indices[2]
            };

            array_insert(faces_clip, f);
            array_insert(normals_clip, normal);
            array_insert(material_indices_clip, material_index);

            continue;
        }
        if (!p0_in && !p1_in && !p2_in)
        {
            continue;
        }

        if (p0_in)
        {
            new_vertex_indices[num_clipped_vertices] = vertices_clip.used;
            num_clipped_vertices++;
            array_insert(vertices_clip, p0);
        }

        if (p0_in != p1_in)
        {
            f32 t = d0 / (d0 - d1);
            v3 intersection = 
            {
                p0.x + t * (p1.x - p0.x),
                p0.y + t * (p1.y - p0.y),
                p0.z + t * (p1.z - p0.z)
            };

            new_vertex_indices[num_clipped_vertices] = vertices_clip.used;
            num_clipped_vertices++;
            array_insert(vertices_clip, intersection);
        }

        if (p1_in)
        {
            new_vertex_indices[num_clipped_vertices] = vertices_clip.used;
            num_clipped_vertices++;
            array_insert(vertices_clip, p1);
        }

        if (p1_in != p2_in)
        {
            f32 t = d1 / (d1 - d2);
            v3 intersection = 
            {
                p1.x + t * (p2.x - p1.x),
                p1.y + t * (p2.y - p1.y),
                p1.z + t * (p2.z - p1.z)
            };

            new_vertex_indices[num_clipped_vertices] = vertices_clip.used;
            num_clipped_vertices++;
            array_insert(vertices_clip, intersection);
        }

        if (p2_in)
        {
            new_vertex_indices[num_clipped_vertices] = vertices_clip.used;
            num_clipped_vertices++;
            array_insert(vertices_clip, p2);
        }

        if (p2_in != p0_in)
        {
            f32 t = d2 / (d2 - d0);
            v3 intersection = 
            {
                p2.x + t * (p0.x - p2.x),
                p2.y + t * (p0.y - p2.y),
                p2.z + t * (p0.z - p2.z)
            };

            new_vertex_indices[num_clipped_vertices] = vertices_clip.used;
            num_clipped_vertices++;
            array_insert(vertices_clip, intersection);
        }

        if (num_clipped_vertices == 3)
        {
            face f = 
            {
                new_vertex_indices[0],
                new_vertex_indices[1],
                new_vertex_indices[2]
            };

            array_insert(faces_clip, f);
            array_insert(normals_clip, normal);
            array_insert(material_indices_clip, material_index);

        }else if(num_clipped_vertices == 4)
        {
            face f0 = 
            {
                new_vertex_indices[0],
                new_vertex_indices[1],
                new_vertex_indices[2]
            };

            face f1 = 
            {
                new_vertex_indices[0],
                new_vertex_indices[2],
                new_vertex_indices[3]
            };

            array_insert(faces_clip, f0);
            array_insert(normals_clip, normal);
            array_insert(material_indices_clip, material_index);
            
            array_insert(faces_clip, f1);
            array_insert(normals_clip, normal);
            array_insert(material_indices_clip, material_index);
        }
    }
    array_copy(v3, vertices_clip, scene.vertices);
    array_copy(face, faces_clip, scene.faces);
    array_copy(v3, normals_clip, scene.normals);
    array_copy(u32, material_indices_clip, scene.material_indices);


    array_clear(vertices_clip);
    array_clear(faces_clip);
    array_clear(normals_clip);
    array_clear(material_indices_clip);
}

void scene_clip_volume()
{
    scene_clip_plane(viewport.near);
    scene_clip_plane(viewport.far);
    scene_clip_plane(viewport.right);
    scene_clip_plane(viewport.left);
    scene_clip_plane(viewport.top);
    scene_clip_plane(viewport.bottom);
}

void scene_render()
{
    array_init(v3, scene.projected);

    for (u32 i = 0; i < scene.vertices.used; i++)
    {
        v3 vert = scene.vertices.vals[i];
        v3 point = {
            round((vert.x / vert.z * viewport.t.scale.z) * (CANVAS_WIDTH / viewport.t.scale.x)),
            round((vert.y / vert.z * viewport.t.scale.z) * (CANVAS_HEIGHT / viewport.t.scale.y)),
            vert.z
        };

        array_insert(scene.projected, point);
    }

    for (u32 i = 0; i < scene.faces.used; i++)
    {
        raster_triangle_solid(i);
        //raster_triangle_wireframe(i);
    }

    array_clear(scene.projected);
}
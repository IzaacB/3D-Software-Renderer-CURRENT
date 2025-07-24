#include "vector.h"

v3 v3_transform(v3 v, transform t, unsigned int mode)
{
    v3 temp;

    switch (mode)
    {
    case 0: // Object transform.
        temp.x = v.x * t.scale.x;
        temp.y = v.y * t.scale.y;
        temp.z = v.z * t.scale.z;
        v = temp;

        temp.x = cos(t.rotation.y) * v.x + sin(t.rotation.y) * v.z;
        temp.z = -sin(t.rotation.y) * v.x + cos(t.rotation.y) * v.z;
        v.x = temp.x;
        v.z = temp.z;

        temp.y = cos(t.rotation.x) * v.y - sin(t.rotation.x) * v.z;
        temp.z = sin(t.rotation.x) * v.y + cos(t.rotation.x) * v.z;
        v.y = temp.y;
        v.z = temp.z;

        temp.x = cos(t.rotation.z) * v.x - sin(t.rotation.z) * v.y;
        temp.y = sin(t.rotation.z) * v.x + cos(t.rotation.z) * v.y;
        v.x = temp.x;
        v.y = temp.y;

        v = v3_add(v, t.position);

        return v;

        break;

    case 1: // Viewport transform.
        v = v3_sub(v, t.position);

        temp.x = cos(-t.rotation.y) * v.x + sin(-t.rotation.y) * v.z;
        temp.z = -sin(-t.rotation.y) * v.x + cos(t.rotation.y) * v.z;
        v.x = temp.x;
        v.z = temp.z;

        temp.y = cos(-t.rotation.x) * v.y - sin(-t.rotation.x) * v.z;
        temp.z = sin(-t.rotation.x) * v.y + cos(-t.rotation.x) * v.z;
        v.y = temp.y;
        v.z = temp.z;

        temp.x = cos(-t.rotation.z) * v.x - sin(-t.rotation.z) * v.y;
        temp.y = sin(-t.rotation.z) * v.x + cos(-t.rotation.z) * v.y;
        v.x = temp.x;
        v.y = temp.y;

        return v;

        break;

    case 2: // Normal object transform.
        temp.x = cos(t.rotation.y) * v.x + sin(t.rotation.y) * v.z;
        temp.z = -sin(t.rotation.y) * v.x + cos(t.rotation.y) * v.z;
        v.x = temp.x;
        v.z = temp.z;

        temp.y = cos(t.rotation.x) * v.y - sin(t.rotation.x) * v.z;
        temp.z = sin(t.rotation.x) * v.y + cos(t.rotation.x) * v.z;
        v.y = temp.y;
        v.z = temp.z;

        temp.x = cos(t.rotation.z) * v.x - sin(t.rotation.z) * v.y;
        temp.y = sin(t.rotation.z) * v.x + cos(t.rotation.z) * v.y;
        v.x = temp.x;
        v.y = temp.y;

        return v;

        break;

    case 3: // Normal viewport transform.
        temp.x = cos(-t.rotation.y) * v.x + sin(-t.rotation.y) * v.z;
        temp.z = -sin(-t.rotation.y) * v.x + cos(t.rotation.y) * v.z;
        v.x = temp.x;
        v.z = temp.z;

        temp.y = cos(-t.rotation.x) * v.y - sin(-t.rotation.x) * v.z;
        temp.z = sin(-t.rotation.x) * v.y + cos(-t.rotation.x) * v.z;
        v.y = temp.y;
        v.z = temp.z;

        temp.x = cos(-t.rotation.z) * v.x - sin(-t.rotation.z) * v.y;
        temp.y = sin(-t.rotation.z) * v.x + cos(-t.rotation.z) * v.y;
        v.x = temp.x;
        v.y = temp.y;

        return v;

        break;
    }
}

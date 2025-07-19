#ifndef OBJECT_H
#define OBJECT_H

extern object object_import(char *path);
extern void object_deport(object o);
extern void object_draw(object);

#endif
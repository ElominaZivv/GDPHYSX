#include "Line.h"

Line::Line(Object* anchor, physics::P6Particle* p) {
    physics::Vector anchorPos = anchor->getObjPos();
    x1 = anchorPos.x;
    y1 = anchorPos.y;

    x2 = p->pos.x;
    y2 = p->pos.y;
}

void Line::update(physics::P6Particle* p) {

    x2 = p->pos.x;
    y2 = p->pos.y;
}

void Line::draw() {
    glBegin(GL_LINES);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
    glEnd();
}
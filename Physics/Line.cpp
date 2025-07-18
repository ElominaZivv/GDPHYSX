#include "Line.h"

Line::Line(Object* anchor, physics::P6Particle* p) {
    physics::Vector anchorPos = anchor->getObjPos();
    x1 = anchorPos.x;
    y1 = anchorPos.y;

    x2 = p->pos.x;
    y2 = p->pos.y;

    initialize(); 
    updateVertexData();
}

void Line::update(physics::P6Particle* p) {

    x2 = p->pos.x;
    y2 = p->pos.y;

    updateVertexData();
}

void Line::draw() {
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(0);
}

void Line::initialize() {
    float vertices[4] = { x1, y1, x2, y2 };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Line::updateVertexData() {
    float vertices[4] = { x1, y1, x2, y2 };

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
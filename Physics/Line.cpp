#include "Line.h"

Line::Line(Object* anchor, physics::P6Particle* p) {
    //cout << "anchor passed: " << anchor->getObjPos().x <<endl;
    start = anchor->getObjPos();
    start.x = -85;
    sphere = p;
    end = sphere->pos;

    initialize(); 
    updateVertexData();
}

void Line::update() {

    end = sphere->pos;

    updateVertexData();
}

void Line::draw() {

    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(0);
}

void Line::initialize() {

    float vertices[6] = {
        start.x, start.y, start.z,
        end.x, end.y, end.z
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Allocate space
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), nullptr, GL_DYNAMIC_DRAW);

    // Vertex attribute: location = 0, vec3
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Line::updateVertexData() {
    float vertices[6] = {
        start.x, start.y, start.z,
        end.x, end.y, end.z
    };

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Line::~Line() {
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}
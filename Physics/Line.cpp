#include "Line.h"

Line::Line(physics::P6Particle* _anchor, physics::P6Particle* p) {

    transform_matrix = mat4(1.f);
    sphere = p;

    //anchor position
    anchor = _anchor;

    initialize(); 
    updateVertexData();
}

void Line::update() {

    //update the end of the line to the spheres position
    start = anchor->pos;
    end = sphere->pos;
    updateVertexData();
}

void Line::draw(Shader shader, Camera camera) {

    shader.activate();

    unsigned int viewLoc = glGetUniformLocation(shader.getShader(), "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(camera.getViewMatrix()));

    unsigned int projLoc = glGetUniformLocation(shader.getShader(), "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(camera.getProjectionMatrix()));

    unsigned int transformLoc = glGetUniformLocation(shader.getShader(), "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, value_ptr(transform_matrix));

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
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "boidManager.h"
#include <vector>
#include <chrono>
#include <thread>

#include <iostream> 

// Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
//Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";

//std::vector<vector_2> boidCenters{
//};

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		double xpos, ypos;
		//getting cursor position
		glfwGetCursorPos(window, &xpos, &ypos);
		std::cout << "Cursor Position at (" << xpos << " : " << ypos << std::endl;
	}
}

int main()
{	
	boidManager boids(1000);

	// (1) GLFW: Initialise & Configure
	// -----------------------------------------
	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	int monitor_width = mode->width; // Monitor's width.
	int monitor_height = mode->height;

	int window_width = (int)(monitor_width); // Window size will be 50% the monitor's size...
	int window_height = (int)(monitor_height); // ... Cast is simply to silence the compiler warning.

	double xpos, ypos;

	GLFWwindow* window = glfwCreateWindow(window_width, window_height, "Boids", NULL, NULL);
	// GLFWwindow* window = glfwCreateWindow(window_width, window_height, "Drawing Basic Shapes - Buffer Objects & Shaders", glfwGetPrimaryMonitor(), NULL); // Full Screen Mode ("Alt" + "F4" to Exit!)

	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Introduce the window into the current context
	glfwMakeContextCurrent(window);
	glfwSetMouseButtonCallback(window, mouse_button_callback);


	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	glViewport(0, 0, window_width, window_height);


	// Create Vertex Shader Object and get its reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach Vertex Shader source to the Vertex Shader Object
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(vertexShader);

	// Create Fragment Shader Object and get its reference
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Attach Fragment Shader source to the Fragment Shader Object
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(fragmentShader);

	// Create Shader Program Object and get its reference
	GLuint shaderProgram = glCreateProgram();
	// Attach the Vertex and Fragment Shaders to the Shader Program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	// Wrap-up/Link all the shaders together into the Shader Program
	glLinkProgram(shaderProgram);

	// Delete the now useless Vertex and Fragment Shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	std::vector<GLfloat> v_vertices;
	v_vertices = boids.getAllVert(window_width, window_height);
	GLfloat* vertices = &v_vertices[0];

	std::vector<GLfloat> bounds_vertices;
	bounds_vertices = boids.getBoundVertices(window_width, window_height);
	GLfloat* lines = &bounds_vertices[0];

	std::vector<GLfloat> quads_ver;
	quads_ver = boids.getQuadVert(window_width, window_height);
	GLfloat* quads_ref = &quads_ver[0];

	std::vector<GLfloat> all_vertices;
	all_vertices = v_vertices;
	all_vertices.insert(all_vertices.end(), bounds_vertices.begin(), bounds_vertices.end());
	all_vertices.insert(all_vertices.end(), quads_ver.begin(), quads_ver.end());
	GLfloat* all_vertices_ptr = &all_vertices[0];

	// Vertices coordinates
	/*GLfloat vertices[] =
	{
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower left corner
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower right corner
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f // Upper corner
	};
	*/
	

	// Create reference containers for the Vartex Array Object and the Vertex Buffer Object
	GLuint VAO, VBO;

	// Generate the VAO and VBO with only 1 object each
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Make the VAO the current Vertex Array Object by binding it
	glBindVertexArray(VAO);

	// Bind the VBO specifying it's a GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Introduce the vertices into the VBO
	glBufferData(GL_ARRAY_BUFFER, (v_vertices.size() + bounds_vertices.size() + quads_ver.size()) * sizeof(GLfloat), all_vertices_ptr, GL_STREAM_DRAW);

	// Configure the Vertex Attribute so that OpenGL knows how to read the VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	// Enable the Vertex Attribute so that OpenGL knows to use it
	glEnableVertexAttribArray(0);

	// Bind both the VBO and VAO to 0 so that we don't accidentally modify the VAO and VBO we created
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);

	const double target_framerate = 60.0;
	const double maxPeriod = 1.0 / target_framerate;
	auto start = std::chrono::steady_clock::now();
	int frames = 0;
	double lastTime = 0.0;


	double x_norm, y_norm;

	boids.setAI('b', boids.boids);
	//boids.addBoid({ 100, 100, 0 });

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		double time = glfwGetTime();
		double deltaTime = time - lastTime;

		if (deltaTime >= maxPeriod) {
			lastTime = time;
			// code here gets called with max FPS
			glfwGetCursorPos(window, &xpos, &ypos);


			boids.updateBoids(xpos, ypos);
			//x_norm = xpos / (window_width / 2) - 1;
			//y_norm = -(ypos / (window_height / 2) - 1);

			v_vertices = boids.getAllVert(window_width, window_height);
			GLfloat* vertices = &v_vertices[0];

			//TODO
			bounds_vertices = boids.getBoundVertices(window_width, window_height);
			lines = &bounds_vertices[0];

			quads_ver = boids.getQuadVert(window_width, window_height);
			quads_ref = &quads_ver[0];

			all_vertices = v_vertices;
			all_vertices.insert(all_vertices.end(), bounds_vertices.begin(), bounds_vertices.end());
			all_vertices.insert(all_vertices.end(), quads_ver.begin(), quads_ver.end());
			all_vertices_ptr = &all_vertices[0];

			glBufferData(GL_ARRAY_BUFFER, (v_vertices.size() + bounds_vertices.size() + quads_ver.size()) * sizeof(GLfloat), all_vertices_ptr, GL_STREAM_DRAW);

			// Specify the color of the background
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			// Clean the back buffer and assign the new color to it
			glClear(GL_COLOR_BUFFER_BIT);
			// Tell OpenGL which Shader Program we want to use
			glUseProgram(shaderProgram);
			// Bind the VAO so OpenGL knows to use it
			glBindVertexArray(VAO);

			// Draw the triangle using the GL_TRIANGLES primitive
			glDrawArrays(GL_TRIANGLES, 0, v_vertices.size() / 3);
			//glDrawArrays(GL_LINES, v_vertices.size() / 3, bounds_vertices.size() / 3);
			glDrawArrays(GL_LINES, v_vertices.size() / 3 + bounds_vertices.size() / 3, quads_ver.size() / 3);
			glfwSwapBuffers(window);
			// Take care of all GLFW events

			std::cout << 1 / deltaTime << std::endl;

			glfwPollEvents();
		}

	}




	// Delete all the objects we've created
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}



/*TODO
1. figure out better rendering system
2. better loop for consistency

*/
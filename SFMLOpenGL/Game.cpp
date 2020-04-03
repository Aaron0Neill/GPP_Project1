#include <Game.h>
#include <Cube.h>

GLuint	vsid,		// Vertex Shader ID
		fsid,		// Fragment Shader ID
		progID,		// Program ID
		vao = 0,	// Vertex Array ID
		vbo,		// Vertex Buffer ID
		vib,		// Vertex Index Buffer
		to,			// Texture ID 1 to 32
		positionID,	// Position ID
		colorID,	// Color ID
		textureID,	// Texture ID
		uvID,		// UV ID
		mvpID;

//const string filename = ".//Assets//Textures//coordinates.tga";
//const string filename = ".//Assets//Textures//cube.tga";
const string filename = ".//Assets//Textures//grid.tga";
//const string filename = ".//Assets//Textures//grid_wip.tga";
//const string filename = ".//Assets//Textures//minecraft.tga";
//const string filename = ".//Assets//Textures//texture.tga";
//const string filename = ".//Assets//Textures//texture_2.tga";
//const string filename = ".//Assets//Textures//uvtemplate.tga";


int width;			// Width of texture
int height;			// Height of texture
int comp_count;		// Component of texture

unsigned char* img_data;		// image data

mat4 mvp[MAX_CUBES], projection, view, model;			// Model View Projection

Game::Game() : 
	window(VideoMode(800, 600), 
	"Introduction to OpenGL Texturing")
{
	for (int index = 0; index < GAME_OBJECTS; index++)
	{
		m_enemyObjects[index] = new GameObject(m_gameObjectStartPos);
	}
}

Game::Game(sf::ContextSettings settings) : 
	window(VideoMode(800, 600), 
	"Introduction to OpenGL Texturing", 
	sf::Style::Default, 
	settings)
{
	m_gameObjectStartPos = vec3{ -14, -4, -4 };
	glm::vec3 pos = m_gameObjectStartPos;
	for (int index = 0; index < GAME_OBJECTS; index++)
	{
		m_enemyObjects[index] = new GameObject(pos);
		m_enemyObjects[index]->setState(CubeState::alive);
		pos.x -= 2.5;
	}

	pos = vec3(-12, -8, -4);

	for (int index = 0; index < GROUND_OBJECTS; index++)
	{
		m_groundObjects[index] = new GameObject(pos);
		m_groundObjects[index]->setState(CubeState::idle);
		m_groundObjects[index]->setScale(3);
		pos.x += 2 * 3; 
	}

	m_gravity = glm::vec3{ 0,.0598 * m_numberScalar,0 };
	m_velocity = glm::vec3(10 * m_numberScalar, 0,0);

	mouseRadius = 50.0f;

	cubeRadius = 150.0f;

	totalRadius = mouseRadius + cubeRadius;

}

Game::~Game()
{
	for (int index = 0; index < GAME_OBJECTS; index++)
	{
		delete m_enemyObjects[index];
	}
	for (int i = 0; i < GROUND_OBJECTS; i++)
	{
		delete m_groundObjects[i];
	}
}


void Game::run()
{

	initialize();

	Event event;

	while (isRunning){

#if (DEBUG >= 2)
		DEBUG_MSG("Game running...");
#endif

		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				isRunning = false;
			}
			
			if (event.type == Event::MouseButtonPressed)
			{
				if (event.key.code == Mouse::Button::Left)
				{
					for (int index = 0; index < GAME_OBJECTS; index++)
					{// check all the cubes against there hit boxes
						sf::Vector2f cubePos{ calculateScreenPos(m_enemyObjects[index]->getPositon()) };

						sf::Vector2f mousePos{ static_cast<sf::Vector2f>(Mouse::getPosition(window)) };

						sf::Vector2f collisionVec{ cubePos - mousePos };

						if (vectorLength(collisionVec) < totalRadius)
						{
							m_enemyObjects[index]->setState(CubeState::dead);
							m_enemyObjects[index]->setVelocity(glm::vec3(0, 100, 75));
						}
					}
				}
			}
		}
		update();
		render();
	}

#if (DEBUG >= 2)
	DEBUG_MSG("Calling Cleanup...");
#endif
	unload();

}

void Game::initialize()
{
	isRunning = true;
	GLint isCompiled = 0;
	GLint isLinked = 0;

	if (!glewInit())
	{
		DEBUG_MSG("glewInit() failed"); 
	}

	DEBUG_MSG(glGetString(GL_VENDOR));
	DEBUG_MSG(glGetString(GL_RENDERER));
	DEBUG_MSG(glGetString(GL_VERSION));

	glGenVertexArrays(1, &vao); //Gen Vertex Array
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);		//Gen Vertex Buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//Vertices (3) x,y,z , Colors (4) RGBA, UV/ST (2)
	glBufferData(GL_ARRAY_BUFFER, ((3 * VERTICES) + (4 * COLORS) + (2 * UVS)) * sizeof(GLfloat), NULL, GL_STATIC_DRAW);

	glGenBuffers(1, &vib); //Gen Vertex Index Buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vib);

	//Indices to be drawn
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * INDICES * sizeof(GLuint), indices, GL_STATIC_DRAW);

	std::ifstream shader("assets//shaders//VertexShader.txt");

	try
	{
		if (!shader.is_open())
		{
			throw std::exception("VertexShader wasnt opened");
		}
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}

	std::istreambuf_iterator<char> shaderStart(shader), shaderEnd;
	std::vector<char> shaderFile(shaderStart, shaderEnd);
	const char* vs_src = &shaderFile[0];

	DEBUG_MSG("Setting Up Vertex Shader");

	vsid = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vsid, 1, (const GLchar**)&vs_src, NULL);
	glCompileShader(vsid);

	//Check is Shader Compiled
	glGetShaderiv(vsid, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_TRUE) {
		DEBUG_MSG("Vertex Shader Compiled");
		isCompiled = GL_FALSE;
	}
	else
	{
		DEBUG_MSG("ERROR: Vertex Shader Compilation Error");
	}

	ifstream fragment("assets//shaders//FragmentShader.txt");

	try
	{
		if (!fragment.is_open())
		{
			throw std::exception("Fragment shader wasnt opened");
		}
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
	std::istreambuf_iterator<char> fragmentStart(fragment), fragmentEnd;
	std::vector<char> fragmentFile(fragmentStart, fragmentEnd);
	const char* fs_src = &fragmentFile[0];

	DEBUG_MSG("Setting Up Fragment Shader");

	fsid = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fsid, 1, (const GLchar**)&fs_src, NULL);
	glCompileShader(fsid);

	//Check is Shader Compiled
	glGetShaderiv(fsid, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_TRUE) {
		DEBUG_MSG("Fragment Shader Compiled");
		isCompiled = GL_FALSE;
	}
	else
	{
		DEBUG_MSG("ERROR: Fragment Shader Compilation Error");
	}

	DEBUG_MSG("Setting Up and Linking Shader");
	progID = glCreateProgram();
	glAttachShader(progID, vsid);
	glAttachShader(progID, fsid);
	glLinkProgram(progID);

	//Check is Shader Linked
	glGetProgramiv(progID, GL_LINK_STATUS, &isLinked);

	if (isLinked == 1) {
		DEBUG_MSG("Shader Linked");
	}
	else
	{
		DEBUG_MSG("ERROR: Shader Link Error");
	}

	//Use Progam on GPU
	glUseProgram(progID);

	img_data = stbi_load(filename.c_str(), &width, &height, &comp_count, 4);

	if (img_data == NULL)
	{
		DEBUG_MSG("ERROR: Texture not loaded");
	}

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &to);
	glBindTexture(GL_TEXTURE_2D, to);

	//Wrap around
	//https://www.khronos.org/opengles/sdk/docs/man/xhtml/glTexParameter.xml
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	//Filtering
	//https://www.khronos.org/opengles/sdk/docs/man/xhtml/glTexParameter.xml
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Bind to OpenGL
	//https://www.khronos.org/opengles/sdk/docs/man/xhtml/glTexImage2D.xml
	glTexImage2D(
		GL_TEXTURE_2D,			//2D Texture Image
		0,						//Mipmapping Level 
		GL_RGBA,				//GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA, GL_RGB, GL_BGR, GL_RGBA 
		width,					//Width
		height,					//Height
		0,						//Border
		GL_RGBA,				//Bitmap
		GL_UNSIGNED_BYTE,		//Specifies Data type of image data
		img_data				//Image Data
		);

	// Find variables in the shader
	//https://www.khronos.org/opengles/sdk/docs/man/xhtml/glGetAttribLocation.xml
	positionID = glGetAttribLocation(progID, "sv_position");
	colorID = glGetAttribLocation(progID, "sv_color");
	uvID = glGetAttribLocation(progID, "sv_uv");
	textureID = glGetUniformLocation(progID, "f_texture");
	mvpID = glGetUniformLocation(progID, "sv_mvp");

	// Projection Matrix 
	projection = perspective(
		45.0f,					// Field of View 45 degrees
		4.0f / 3.0f,			// Aspect ratio
		5.0f,					// Display Range Min : 0.1f unit
		100.0f					// Display Range Max : 100.0f unit
		);

	// Camera Matrix
	view = lookAt(
		vec3(0.0f, 4.0f, 10.0f),	// Camera (x,y,z), in World Space
		vec3(0.0f, 0.0f, 0.0f),		// Camera looking at origin
		vec3(0.0f, 1.0f, 0.0f)		// 0.0f, 1.0f, 0.0f Look Down and 0.0f, -1.0f, 0.0f Look Up
		);

	// Model matrix
	model = mat4(
		1.0f					// Identity Matrix
		);

	// Enable Depth Test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
}

void Game::update()
{
#if (DEBUG >= 2)
	DEBUG_MSG("Updating...");
#endif

	for (int index = 0; index < GAME_OBJECTS; index++)
	{
		m_enemyObjects[index]->update();	
	}

	// Update Model View Projection
	for (int index = 0; index < GAME_OBJECTS; index++)
	{
		mvp[m_enemyObjects[index]->getModelNum()] = projection * view * m_enemyObjects[index]->getModel();
	}

	for (int index = 0; index < GROUND_OBJECTS; index++)
	{
		mvp[m_groundObjects[index]->getModelNum()] = projection * view * m_groundObjects[index]->getModel();
	}
}

void Game::render()
{

#if (DEBUG >= 2)
	DEBUG_MSG("Render Loop...");
#endif

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Enable Arrays
	glEnableVertexAttribArray(positionID);
	glEnableVertexAttribArray(colorID);
	glEnableVertexAttribArray(uvID);

	for (int i = 0; i < GROUND_OBJECTS; i++)
	{
		renderGround(m_groundObjects[i]);
	}
	
	for (int index = 0; index < GAME_OBJECTS; index++)
	{
		drawCube(m_enemyObjects[index]);
	}

	window.display();

	//Disable Arrays
	glDisableVertexAttribArray(positionID);
	glDisableVertexAttribArray(colorID);
	glDisableVertexAttribArray(uvID);
	
}

void Game::renderGround(GameObject* obj)
{
	//VBO Data....vertices, colors and UV's appended
	glBufferSubData(GL_ARRAY_BUFFER, 0 * VERTICES * sizeof(GLfloat), 3 * VERTICES * sizeof(GLfloat), obj->getVertex());
	glBufferSubData(GL_ARRAY_BUFFER, 3 * VERTICES * sizeof(GLfloat), 4 * COLORS * sizeof(GLfloat), obj->getColour());
	glBufferSubData(GL_ARRAY_BUFFER, ((3 * VERTICES) + (4 * COLORS)) * sizeof(GLfloat), 2 * UVS * sizeof(GLfloat), obj->getUVS());

	// Send transformation to shader mvp uniform
	glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[obj->getModelNum()][0][0]);

	//Set Active Texture .... 32
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(textureID, 0);

	//Set pointers for each parameter (with appropriate starting positions)
	//https://www.khronos.org/opengles/sdk/docs/man/xhtml/glVertexAttribPointer.xml
	glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(colorID, 4, GL_FLOAT, GL_FALSE, 0, (VOID*)(3 * VERTICES * sizeof(GLfloat)));
	glVertexAttribPointer(uvID, 2, GL_FLOAT, GL_FALSE, 0, (VOID*)(((3 * VERTICES) + (4 * COLORS)) * sizeof(GLfloat)));


	//Draw Element Arrays
	glDrawElements(GL_TRIANGLES, 3 * INDICES, GL_UNSIGNED_INT, NULL);
}

void Game::unload()
{
#if (DEBUG >= 2)
	DEBUG_MSG("Cleaning up...");
#endif
	glDeleteProgram(progID);	//Delete Shader
	glDeleteBuffers(1, &vbo);	//Delete Vertex Buffer
	glDeleteBuffers(1, &vib);	//Delete Vertex Index Buffer
	stbi_image_free(img_data);		//Free image
}

float Game::vectorLength(sf::Vector2f t_vec)
{
	float len{ sqrt(t_vec.x * t_vec.x + t_vec.y * t_vec.y) };
	return len;
}

sf::Vector2f Game::calculateScreenPos(glm::vec3 t_pos)
{
	sf::Vector2f pos;
	pos.x = { 800 * (t_pos.x + 10) / 28 };
	pos.y = { 600 * (t_pos.y + 10) / 28 };
	return pos;
}

void Game::drawCube(GameObject* obj)
{
	//VBO Data....vertices, colors and UV's appended
	glBufferSubData(GL_ARRAY_BUFFER, 0 * VERTICES * sizeof(GLfloat), 3 * VERTICES * sizeof(GLfloat), obj->getVertex());
	glBufferSubData(GL_ARRAY_BUFFER, 3 * VERTICES * sizeof(GLfloat), 4 * COLORS * sizeof(GLfloat), obj->getColour());
	glBufferSubData(GL_ARRAY_BUFFER, ((3 * VERTICES) + (4 * COLORS)) * sizeof(GLfloat), 2 * UVS * sizeof(GLfloat), obj->getUVS());

	// Send transformation to shader mvp uniform
	glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[obj->getModelNum()][0][0]);

	//Set Active Texture .... 32
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(textureID, 0);

	//Set pointers for each parameter (with appropriate starting positions)
	//https://www.khronos.org/opengles/sdk/docs/man/xhtml/glVertexAttribPointer.xml
	glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(colorID, 4, GL_FLOAT, GL_FALSE, 0, (VOID*)(3 * VERTICES * sizeof(GLfloat)));
	glVertexAttribPointer(uvID, 2, GL_FLOAT, GL_FALSE, 0, (VOID*)(((3 * VERTICES) + (4 * COLORS)) * sizeof(GLfloat)));

	//Draw Element Arrays
	glDrawElements(GL_TRIANGLES, 3 * INDICES, GL_UNSIGNED_INT, NULL);
}
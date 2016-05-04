#include "GameAssetManager.h"



GameAssetManager::GameAssetManager(ApplicationMode mode) {
  std::string vertex_shader("shaders/translate.vs");
  std::string fragment_shader("shaders/fragment.fs");

  switch(mode) {
  case ROTATE:
    vertex_shader = "shaders/rotate.vs";
    break;
  case SCALE:
    vertex_shader = "shaders/scale.vs";
    break;
  case TRANSFORM:
  default:
    break;
  };

  program_token = CreateGLProgram(vertex_shader, fragment_shader);

// link to the uniform variables in the translate shader

projection_matrix_link = glGetUniformLocation(program_token, "projection_matrix");
translate_matrix_link = glGetUniformLocation(program_token, "translate_matrix");
view_matrix_link = glGetUniformLocation(program_token, "view_matrix");


	// create the matrix based on the window size // used to solve z buffering
projection_matrix = glm::perspective(glm::radians(45.0f), (float) 640/ (float) 480, 0.1f, 1000.0f);

}


// communicates with camera class
void GameAssetManager::UpdateCameraPosition(Input input_direction,  int mouse_x, int mouse_y){


 view_matrix = camera.UpdateCameraPosition(input_direction, mouse_x, mouse_y);

  }

GameAssetManager::~GameAssetManager() {
  glDeleteProgram(program_token);
}

/**
 * Adds a GameAsset to the scene graph.
 */
void GameAssetManager::AddAsset(std::shared_ptr<GameAsset> the_asset) {
  draw_list.push_back(the_asset);
}

/**
 * Draws each GameAsset in the scene graph.
 */
void GameAssetManager::Draw() {
  for(auto ga: draw_list) {


/// before drawing an asset , update the matrix values in the translate shader
	glUniformMatrix4fv(projection_matrix_link, 1, GL_FALSE, &projection_matrix[0][0]);
	glUniformMatrix4fv(view_matrix_link, 1, GL_FALSE, &view_matrix[0][0]);

	translate_matrix= ga->GetModelTransformation();
	glUniformMatrix4fv(translate_matrix_link, 1, GL_FALSE, &translate_matrix[0][0]);


		bounding_box1_max = ga->GetMaxAndMin(1);
		bounding_box1_min = ga->GetMaxAndMin(2);
		bounding_box1_position = ga->GetVec3();

		for(auto ga2: draw_list)
		{
            bounding_box2_max = ga2->GetMaxAndMin(1);
            bounding_box2_min = ga2->GetMaxAndMin(2);
            bounding_box2_position = ga2->GetVec3();

            //If the two bounding boxes are not in the same position (not the same bounding box), then check for a collision.
            if( bounding_box1_position != bounding_box2_position){
                ga -> CheckCollision(bounding_box1_max, bounding_box1_min, bounding_box2_max, bounding_box2_min);
            }
		}

    ga->Draw(program_token);
  }
}


GLuint GameAssetManager::CreateGLProgram(std::string & vertex_shader
                                         , std::string & fragment_shader) {
  auto v_shader_token = CreateGLESShader(GL_VERTEX_SHADER, vertex_shader);
  auto f_shader_token = CreateGLESShader(GL_FRAGMENT_SHADER, fragment_shader);

  GLint program_ok;

  GLuint program = glCreateProgram();

  glAttachShader(program, v_shader_token);
  glAttachShader(program, f_shader_token);
  glLinkProgram(program);

  glGetProgramiv(program, GL_LINK_STATUS, &program_ok);
  if (!program_ok) {
    std::cerr << "Failed to link shader program:" << std::endl;
    glDeleteProgram(program);
    exit(-1);
  }
  return program;
}


GLuint GameAssetManager::CreateGLESShader(GLenum type, std::string & shader) {
  GLuint shader_token;
  GLint shader_ok;
  auto source = ReadShader(shader);

  if (!source.first)
    return 0;

  shader_token = glCreateShader(type);
  glShaderSource(shader_token, 1, (const GLchar**)&source.first, &source.second);
  glCompileShader(shader_token);
  delete(source.first);

  glGetShaderiv(shader_token, GL_COMPILE_STATUS, &shader_ok);
  if (!shader_ok) {
    GLint maxLength = 0;
    glGetShaderiv(shader_token, GL_INFO_LOG_LENGTH, &maxLength);

    //The maxLength includes the NULL character
    std::vector<char> errorLog(maxLength);
    glGetShaderInfoLog(shader_token, maxLength, &maxLength, &errorLog[0]);

    //Provide the infolog in whatever manor you deem best.
    std::cerr << "Failed to compile " << shader << " with error code " << shader_ok << std::endl;
    for(auto c: errorLog) {
      std::cerr << c;
    }

    glDeleteShader(shader_token); //Don't leak the shader.
    exit(-1);
  }
  return shader_token;
}

std::pair<GLchar *, GLint> GameAssetManager::ReadShader(std::string & shader) {
  std::ifstream input_file;
  GLint length;
  input_file.open(shader, std::ios::in);

  input_file.seekg(0, std::ios::end);  // go to the end of the file
  length = input_file.tellg();    // get length of the file
  input_file.seekg(0, std::ios::beg);  // go to beginning of the file

  GLchar * buffer = new GLchar[length+1];
  input_file.read(buffer, length);
  buffer[length+1]='\0';  // Ensure null terminated

  input_file.close();
  return std::make_pair(buffer, length);
}

GameAssetManager::GameAssetManager(GameAssetManager const& the_manager) {
  // TODO: implement this
}

GameAssetManager::GameAssetManager(GameAssetManager const&& the_manager) {
  // TODO: implement this
}

void GameAssetManager::operator=(GameAssetManager const& the_manager) {
  // TODO: implement this
}

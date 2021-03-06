#include "GameWorld.h"

GameWorld::GameWorld (ApplicationMode mode) {


  asset_manager = std::make_shared<GameAssetManager>(mode);
  colour_manager.AddColour("random", glm::vec3(-0.1, -0.1, -0.1));

  // Position, Colour, Type of animation, Scale, Rotation, Speed

  //Collision check, moving and spinning animation
	asset_manager->AddAsset(std::make_shared<CubeAsset>(glm::vec3(-3.0, -3.0, 0.0),colour_manager.GetColour("random"), 4, 1, glm::vec3(0.0,0.0,0.0), glm::vec3(0.0,-0.04,0.0)));
	asset_manager->AddAsset(std::make_shared<CubeAsset>(glm::vec3(-3.0, -6.0, 0.0),colour_manager.GetColour("random"), 2, 1, glm::vec3(0.0,0.0,0.0), glm::vec3(0.0,0.0,0.0)));
	asset_manager->AddAsset(std::make_shared<CubeAsset>(glm::vec3(-6.0, 0.0, 0.0),colour_manager.GetColour("random"), 4, 1, glm::vec3(0.0,0.0,0.0), glm::vec3(-0.04,0.0,0.0)));
  asset_manager->AddAsset(std::make_shared<CubeAsset>(glm::vec3(-9.0, 0.0, 0.0),colour_manager.GetColour("random"), 2, 1, glm::vec3(0.0,0.0,0.0), glm::vec3(0.0,0.0,0.0)));
  
  //Box Enlargment
  asset_manager->AddAsset(std::make_shared<CubeAsset>(glm::vec3(6.0, 10.0, 3.0),colour_manager.GetColour("random"), 3, 5, glm::vec3(0.0,0.0,0.0), glm::vec3(0.0,0.0,0.0)));

  //Box Enlargment and spining
  asset_manager->AddAsset(std::make_shared<CubeAsset>(glm::vec3(10.0, 7.0, 0.0),colour_manager.GetColour("random"), 5, 3, glm::vec3(0.0,0.0,0.0), glm::vec3(0.0,0.0,0.0)));

}

  // calls the draw method in GameAssetManager.cc
	void GameWorld::Draw() {
  asset_manager->Draw();
}

  // calls the updatecamera method in GameAssetManager passing the user input and mouse coordnates.

	void GameWorld::UpdateCameraPosition(Input input_direction, int mouse_x, int mouse_y){
	asset_manager->UpdateCameraPosition(input_direction,  mouse_x,  mouse_y);
}

#include "BoundingBox.h"

BoundingBox::BoundingBox(glm::vec3 position, int type, float scale , glm::vec3 rotation, glm::vec3 speed)
{
	this->position = position -= glm::vec3(0.5f,0.5f,0.5f);
	this->type = type;
	this->scale = scale;
	this->rotation = rotation;
	this->speed = speed;

	std::cout << "Initialised BoundingBox at point: [" << position.x << "," << position.y << "," << position.z << "]" << std::endl;
}

glm::mat4 BoundingBox::GetModelTransformation()
{
    if (this->type == 1)
	{
		this->Translate(glm::vec3(0.01f, 0.0f, 0.0f));
	}
    else if (this->type == 2)
	{
		this->Rotate(glm::vec3(0.01f, 0.01f, 0.01f));
	}
    else if(this->type == 3)
	{
		this->Scale(0.01f);
	}
    else if(this->type == 4)
	{
		this->Translate(glm::vec3(this->speed.x,this->speed.y,this->speed.z));
	}
	else if(this->type == 5)
	{
		this->Scale(0.01f);
		this->Rotate(glm::vec3(0.05f, 0.01f, 0.01f));
	}

    //Scale rotate and translate a bounding box and return the updated matrix.
    glm::mat4 scale_matrix = glm::scale(glm::vec3(this->scale, this->scale, this->scale));
	glm::mat4 translate_matrix = glm::translate(glm::mat4(), glm::vec3(this->position.x, this->position.y, this->position.z));

	model_matrix = translate_matrix * scale_matrix;
    model_matrix = glm::rotate(model_matrix, rotation.x, glm::vec3(1, 0, 0));
	model_matrix = glm::rotate(model_matrix, rotation.y, glm::vec3(0, 1, 0));
    model_matrix = glm::rotate(model_matrix, rotation.z, glm::vec3(0, 0, 1));

	return model_matrix;
}


/**
 * Used to translate by a certain speed
 */
void BoundingBox::Translate(glm::vec3 translate_speed)
{


	glm::vec3 new_position;
	
		new_position = this->position + translate_speed;


	this->position = new_position;
}



void BoundingBox::Scale(float scale_speed)
{
 

	float new_scale;
	if(this->scale > 5.0f)
	{
		new_scale = 1.0f;
	}
	else
	{
		new_scale = this->scale + scale_speed;
	}

	this->scale = new_scale;
}



void BoundingBox::Rotate(glm::vec3 rotate_speed)
{
	glm::vec3 new_rotation;
	new_rotation = this->rotation + rotate_speed;
	
	this->rotation = new_rotation;
}



glm::vec3 BoundingBox::GetVec3()
{
	return position;
}


glm::vec3 BoundingBox::GetMaxAndMin(int type)
{
    glm::vec3 bounds;

    if (type == 1)
    {
    // return max bounds
    bounds = this->position += glm::vec3(1.1f * this-> scale ,1.1f* this-> scale,1.1f* this-> scale);

    }
    else if (type == 2)
    {
    // return minimum bounds
     bounds = this->position += glm::vec3(-1.1f* this-> scale,-1.1f* this-> scale,-1.1f* this-> scale);
    }
	return bounds;
}



void BoundingBox::CheckCollision(glm::vec3 bounding_box1_max, glm::vec3 bounding_box1_min, glm::vec3 bounding_box2_max, glm::vec3 bounding_box2_min)
{
    //Check if Box1's max is greater than Box2's min and Box1's min is less than Box2's max
    if (bounding_box1_max.x > bounding_box2_min.x && bounding_box1_min.x < bounding_box2_max.x &&
        bounding_box1_max.y > bounding_box2_min.y && bounding_box1_min.y < bounding_box2_max.y &&
        bounding_box1_max.z > bounding_box2_min.z && bounding_box1_min.z < bounding_box2_max.z)
        {
            std::cout << "Collision between 2 boxes" << std::endl;
            std::cout << "boxes changing direction" << std::endl;
            std::cout << "" << std::endl;
            glm::vec3 reverse_speed = this->speed;
            reverse_speed = reverse_speed + reverse_speed;
            this->speed -= reverse_speed;
        }
    else
        {
        }
}

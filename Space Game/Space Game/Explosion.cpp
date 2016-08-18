#include "Explosion.h"

#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

/*----------------------------------------*/
/*		Methods for Class ExplAnim		  */
/*----------------------------------------*/

ExplAnim::ExplAnim()
{
	// Empty
}

ExplAnim::ExplAnim(glm::vec3 position, glm::vec3 direction, Texture2D texture)
{
	_sprite = SpriteMap(texture, 0.4f, 0.4f, position, 0.0025f, OTHER);
	_sprite.SetDirection(direction);
	lifetime = 99;
	currentTexture = 0;
}

ExplAnim::~ExplAnim()
{
	// Empty
}

void ExplAnim::Draw()
{
	_sprite.Draw();
	_sprite.UpdatePosition();
}

void ExplAnim::ChangeTexture(Texture2D texture)
{
	_sprite.SetTexture(texture);
}

void ExplAnim::Delete()
{
	// Empty
}

glm::vec3 ExplAnim::GetPosition()
{
	return  _sprite.GetPosition();
}


/*----------------------------------------*/
/*	    Methods for Class Explosion		  */
/*----------------------------------------*/


Explosion::Explosion()
{

}
Explosion::~Explosion()
{

}

void Explosion::Init(GLuint shader_id)
{
	// Load the textures to memory:

	// RED EXPLOSION
	_textures.emplace_back("../deps/textures/explosionred/Explosion_001.png", PNG_RGBA);
	_textures.emplace_back("../deps/textures/explosionred/Explosion_002.png", PNG_RGBA);
	_textures.emplace_back("../deps/textures/explosionred/Explosion_003.png", PNG_RGBA);
	_textures.emplace_back("../deps/textures/explosionred/Explosion_004.png", PNG_RGBA);
	_textures.emplace_back("../deps/textures/explosionred/Explosion_005.png", PNG_RGBA);
	_textures.emplace_back("../deps/textures/explosionred/Explosion_006.png", PNG_RGBA);
	_textures.emplace_back("../deps/textures/explosionred/Explosion_007.png", PNG_RGBA);
	_textures.emplace_back("../deps/textures/explosionred/Explosion_008.png", PNG_RGBA);
	_textures.emplace_back("../deps/textures/explosionred/Explosion_009.png", PNG_RGBA);
	_textures.emplace_back("../deps/textures/explosionred/Explosion_010.png", PNG_RGBA);

	// PURPLE EXPLOSION
	_textures.emplace_back("../deps/textures/explosionpurple/Explosion_001.png", PNG_RGBA);
	_textures.emplace_back("../deps/textures/explosionpurple/Explosion_002.png", PNG_RGBA);
	_textures.emplace_back("../deps/textures/explosionpurple/Explosion_003.png", PNG_RGBA);
	_textures.emplace_back("../deps/textures/explosionpurple/Explosion_004.png", PNG_RGBA);
	_textures.emplace_back("../deps/textures/explosionpurple/Explosion_005.png", PNG_RGBA);
	_textures.emplace_back("../deps/textures/explosionpurple/Explosion_006.png", PNG_RGBA);
	_textures.emplace_back("../deps/textures/explosionpurple/Explosion_007.png", PNG_RGBA);
	_textures.emplace_back("../deps/textures/explosionpurple/Explosion_008.png", PNG_RGBA);
	_textures.emplace_back("../deps/textures/explosionpurple/Explosion_009.png", PNG_RGBA);
	_textures.emplace_back("../deps/textures/explosionpurple/Explosion_010.png", PNG_RGBA);


	_shader_id = glGetUniformLocation(shader_id, "model");
}

void Explosion::Add(glm::vec3 position, glm::vec3 direction)
{
	_explosion.emplace_back(position, direction, _textures[0]);
}

void Explosion::Draw()
{
	for (int i = 0; i < _explosion.size(); i++)
	{
		// Delete if inactive
		if (_explosion[i].lifetime <= 0) {
			_explosion[i] = _explosion.back();
			_explosion.pop_back();
			i--;
			continue;
		}
		//Change current texture if frame permitting
		else if (_explosion[i].lifetime % 10 == 0)
		{
			_explosion[i].ChangeTexture(_textures[++_explosion[i].currentTexture]);
		}

		glm::mat4 modelExpl;
		modelExpl = glm::translate(modelExpl, _explosion[i].GetPosition());
		glUniformMatrix4fv(_shader_id, 1, GL_FALSE, glm::value_ptr(modelExpl));

		_explosion[i].Draw();

		_explosion[i].lifetime--;
	}

}

void Explosion::DeleteAt(int index)
{
	_explosion[index] = _explosion.back();
	_explosion.pop_back();
}

void Explosion::DeleteAll()
{
	for (int i = 0; i < _textures.size(); i++)
	{
		_textures[i].Delete();
	}
}

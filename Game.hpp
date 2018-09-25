#pragma once

#include <glm/glm.hpp>

struct Game {
	float paddle1 = 90.0f;
  float paddle2 = -90.0f;
	glm::vec2 ball = glm::vec2(0.0f, 0.0f);
	glm::vec2 ball_velocity = glm::vec2(0.0f,0.0f);
  glm::vec2 bullet1 = glm::vec2(0.0f, 10.0f);
  glm::vec2 bullet1_velocity = glm::vec2(0.0f, 0.0f);
  glm::vec2 bullet2 = glm::vec2(0.0f, 10.0f);
  glm::vec2 bullet2_velocity = glm::vec2(0.0f, 0.0f);
  int score1 = 0;
  int score2 = 0;
  bool fire1 = false;
  bool fire2 = false;
  
  bool is_player1 = true;

  bool check_point(glm::vec2 pt);
  void check_score();
  void reset_ball();
	void update(float time);

	static constexpr const float FrameWidth = 10.0f;
	static constexpr const float FrameHeight = 8.0f;
	static constexpr const float PaddleWidth = 2.0f;
	static constexpr const float PaddleHeight = 0.4f;
	static constexpr const float BallRadius = 0.5f;
  const float bullet_startx = 5.0f;
  const float bullet_speed = 5.0f;
};

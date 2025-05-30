#include <raylib.h>
#include <iostream>

int main()
{
  const int windowWidth{600};
  const int windowHeight{480};
  InitWindow(windowWidth, windowHeight, "dash game");

  const int gravity{1'000}; //(pixel / frame)

  Texture2D scarfy = LoadTexture("texture/scarfy.png");
  Rectangle scarfyRec;
  scarfyRec.width = scarfy.width / 6;
  scarfyRec.height = scarfy.height;
  scarfyRec.x = 0;
  scarfyRec.y = 0;
  Vector2 scarfyPos;
  scarfyPos.x = windowWidth / 2 - scarfyRec.width / 2;
  scarfyPos.y = windowHeight - scarfy.height;

  int frame{};
  const float updateTime{1.0 / 12.0};
  float runningTime{0};

  // rect
  int velocity{0};

  bool isInAir;
  // velocidade do pulo (pixel por segundos)
  const int jumpVel{-600};

  SetTargetFPS(60);
  while (!WindowShouldClose())
  {
    // delta time
    const float dT{GetFrameTime()};

    BeginDrawing();
    ClearBackground(WHITE);

    // logica de makako

    if (scarfyPos.y >= windowHeight - scarfyRec.height)
    {
      velocity = 0;
      isInAir = false;
    }
    else
    {
      velocity += gravity * dT;
      isInAir = true;
    }

    if (IsKeyPressed(KEY_SPACE) && !isInAir)
    {
      velocity += jumpVel;
    }

    // atualizar essa porra por frame
    scarfyPos.y += velocity * dT;

    runningTime += dT;
    if (runningTime >= updateTime)
    {
      runningTime = 0;
      scarfyRec.x = frame * scarfyRec.width;
      frame++;
      if (frame > 5)
      {
        frame = 0;
      }
    }

    // Update Frame

    DrawTextureRec(scarfy, scarfyRec, scarfyPos, WHITE);

    // final do código bosta
    EndDrawing();
  }
  UnloadTexture(scarfy);
  CloseWindow();
}
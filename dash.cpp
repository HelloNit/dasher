#include <raylib.h>
#include <iostream>

struct AnimData
{
  Rectangle rec;
  Vector2 pos;
  int frame;
  float updateTime;
  float runningTime;
};

int main()
{
  const int windowWidth{600};
  const int windowHeight{480};
  InitWindow(windowWidth, windowHeight, "dash game");

  const int gravity{1'000}; //(pixels por segundo)

  Texture2D nebula = LoadTexture("texture/12_nebula_spritesheet.png");

  // neblina principal
  AnimData nebData{
      {0.0f, 0.0f, nebula.width / 8.0f, nebula.height / 8.0f},
      {windowWidth, windowHeight - nebula.height / 8.0f},
      0,
      1.0f / 12.0f,
      0.0f};

  // neblina secundária
  AnimData neb2Data{
      {0.0f, 0.0f, nebula.width / 8.0f, nebula.height / 8.0f},
      {windowWidth + 300, windowHeight - nebula.height / 8.0f},
      0,
      1.0f / 16.0f,
      0.0f};

  // velocidade da neblina
  int nebVel{-600};

  Texture2D scarfy = LoadTexture("texture/scarfy.png");

  // personagem principal
  AnimData scarfyData;
  scarfyData.rec.width = scarfy.width / 6.0f;
  scarfyData.rec.height = scarfy.height;
  scarfyData.rec.x = 0.0f;
  scarfyData.rec.y = 0.0f;
  scarfyData.pos.x = windowWidth / 2.0f - scarfyData.rec.width / 2.0f;
  scarfyData.pos.y = windowHeight - scarfyData.rec.height;
  scarfyData.frame = 0;
  scarfyData.updateTime = 1.0f / 12.0f;
  scarfyData.runningTime = 0.0f;

  int velocity{0};
  bool isInAir = false;

  const int jumpVel{-600};

  SetTargetFPS(60);

  while (!WindowShouldClose())
  {
    // inica o desenho
    BeginDrawing();
    ClearBackground(WHITE);

    // delta time
    const float dT{GetFrameTime()};

    // lógica de física
    if (scarfyData.pos.y >= windowHeight - scarfyData.rec.height)
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

    // atualiza posições
    scarfyData.pos.y += velocity * dT;
    nebData.pos.x += nebVel * dT;
    neb2Data.pos.x += nebVel * dT;

    // atualiza animação do personagem
    if (!isInAir)
    {
      scarfyData.runningTime += dT;
      if (scarfyData.runningTime >= scarfyData.updateTime)
      {
        scarfyData.runningTime = 0;
        scarfyData.rec.x = scarfyData.frame * scarfyData.rec.width;
        scarfyData.frame++;
        if (scarfyData.frame > 5)
        {
          scarfyData.frame = 0;
        }
      }
    }

    // atualiza animação da neblina 1
    nebData.runningTime += dT;
    if (nebData.runningTime >= nebData.updateTime)
    {
      nebData.runningTime = 0;
      nebData.rec.x = nebData.frame * nebData.rec.width;
      nebData.frame++;
      if (nebData.frame > 7)
      {
        nebData.frame = 0;
      }
    }

    // atualiza animação da neblina 2
    neb2Data.runningTime += dT;
    if (neb2Data.runningTime >= neb2Data.updateTime)
    {
      neb2Data.runningTime = 0;
      neb2Data.rec.x = neb2Data.frame * neb2Data.rec.width;
      neb2Data.frame++;
      if (neb2Data.frame > 7)
      {
        neb2Data.frame = 0;
      }
    }

    //neblina
    DrawTextureRec(nebula, nebData.rec, nebData.pos, WHITE);
    DrawTextureRec(nebula, neb2Data.rec, neb2Data.pos, ORANGE);

    //personagem
    DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);

    EndDrawing();
  }
  UnloadTexture(scarfy);
  UnloadTexture(nebula);
  CloseWindow();
}

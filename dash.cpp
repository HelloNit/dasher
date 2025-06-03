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

bool isOnGround(AnimData data, int windowHeight)
{
  return data.pos.y >= windowHeight - data.rec.height;
}

AnimData updateAnimData(AnimData data, float deltaTime, int maxFrame)
{
  // update running time
  data.runningTime += deltaTime;
  if (data.runningTime >= data.updateTime)
  {
    data.runningTime = 0.0;
    // update animation frame
    data.rec.x = data.frame * data.rec.width;
    data.frame++;
    if (data.frame > maxFrame)
    {
      data.frame = 0;
    }
  }
  return data;
}

int main()
{
  int windowDimensions[2];
  windowDimensions[0] = 600;
  windowDimensions[1] = 480;

  InitWindow(windowDimensions[0], windowDimensions[1], "dash game");

  const int gravity{1'000}; //(pixels por segundo)

  Texture2D nebula = LoadTexture("texture/12_nebula_spritesheet.png");

  const int sizeOfNebulae{30};
  AnimData nebulae[sizeOfNebulae]{};

  for (int i = 0; i < sizeOfNebulae; i++)
  {
    nebulae[i].rec.x = 0.0;
    nebulae[i].rec.y = 0.0;
    nebulae[i].rec.width = nebula.width / 8;
    nebulae[i].rec.height = nebula.height / 8;
    nebulae[i].pos.y = windowDimensions[1] - nebula.height / 8;
    nebulae[i].frame = 0;
    nebulae[i].runningTime = 0.0;
    nebulae[i].updateTime = 1.16;

    nebulae[i].pos.x = windowDimensions[0] + i * 600;
  }

  float finishLine{nebulae[sizeOfNebulae - 1].pos.x};

  // velocidade da neblina
  int nebVel{-600};

  Texture2D scarfy = LoadTexture("texture/scarfy.png");

  // personagem principal
  AnimData scarfyData;
  scarfyData.rec.width = scarfy.width / 6.0f;
  scarfyData.rec.height = scarfy.height;
  scarfyData.rec.x = 0.0f;
  scarfyData.rec.y = 0.0f;
  scarfyData.pos.x = windowDimensions[0] / 2.0f - scarfyData.rec.width / 2.0f;
  scarfyData.pos.y = windowDimensions[1] - scarfyData.rec.height;
  scarfyData.frame = 0;
  scarfyData.updateTime = 1.0f / 12.0f;
  scarfyData.runningTime = 0.0f;

  int velocity{0};
  bool isInAir = false;
  const int jumpVel{-600};

  Texture2D background = LoadTexture("texture/far-buildings.png");
  float bgX{};
  Texture2D midground = LoadTexture("texture/back-buildings.png");
  float mgX{};
  Texture2D foreground = LoadTexture("texture/foreground.png");
  float fgX{};

  bool collision{};
  SetTargetFPS(60);
  while (!WindowShouldClose())
  {
    // mostra o fps
    DrawFPS(0, 10);
    // delta time
    const float dT{GetFrameTime()};

    // inica o desenho
    BeginDrawing();
    ClearBackground(WHITE);

    bgX -= 60 * dT;
    if (bgX <= -background.width * 2)
    {
      bgX = 0.0;
    }

    mgX -= 40 * dT;
    if (mgX <= -midground.width * 2)
    {
      mgX = 0.0;
    }

    fgX -= 90 * dT;
    if (fgX <= -foreground.width * 2)
    {
      fgX = 0.0;
    }

    // textura background
    Vector2 bg1Pos{bgX, 0.0};
    DrawTextureEx(background, bg1Pos, 0.0, 2.5, WHITE);
    Vector2 bg2Pos{bgX + background.width * 2, 0.0};
    DrawTextureEx(background, bg2Pos, 0.0, 2.5, WHITE);

    // textura mid
    Vector2 mg1Pos{mgX, 0.0};
    DrawTextureEx(midground, mg1Pos, 0.0, 3.0, WHITE);
    Vector2 mg2Pos{mgX + midground.width * 2, 0.0};
    DrawTextureEx(midground, mg2Pos, 0.0, 2.0, WHITE);

    // textura frentola
    Vector2 fg1Pos{fgX, 0.0};
    DrawTextureEx(foreground, fg1Pos, 0.0, 3.0, WHITE);
    Vector2 fg2Pos{fgX + foreground.width * 2, 0.0};
    DrawTextureEx(foreground, fg2Pos, 0.0, 2.0, WHITE);

    // lógica de física
    if (isOnGround(scarfyData, windowDimensions[1]))
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

    for (int i = 0; i < sizeOfNebulae; i++)
    {
      // atualização da posição de cada neblina
      nebulae[i].pos.x += nebVel * dT;
    }

    // linha de chegada
    finishLine += nebVel * dT;

    // atualiza posições
    scarfyData.pos.y += velocity * dT;

    // atualiza animação do personagem
    if (!isInAir)
    {
      scarfyData = updateAnimData(scarfyData, dT, 5);
    }

    for (AnimData nebula : nebulae)
    {
      float pad{50};
      Rectangle nebRec{
          nebula.pos.x + pad,
          nebula.pos.y + pad,
          nebula.rec.width - 2 * pad,
          nebula.rec.height - 2 * pad};
      Rectangle scarfyRec{
          scarfyData.pos.x,
          scarfyData.pos.y,
          scarfyData.rec.width,
          scarfyData.rec.height};

      if (CheckCollisionRecs(nebRec, scarfyRec))
      {
        collision = true;
      }
    }

    if (collision)
    {
      DrawText("Perdeu carai", windowDimensions[0] / 4, windowDimensions[1] / 2, 30, RED);
    }
    else if (scarfyData.pos.x >= finishLine)
    {
      DrawText("Ganhou", windowDimensions[0] / 4, windowDimensions[1] / 2, 30, RED);
    }

    else
    {
      for (int i = 0; i < sizeOfNebulae; i++)
      {
        // neblina
        DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
      }
      // personagem
      DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
    }

    for (int i = 0; i < sizeOfNebulae; i++)
    {
      nebulae[i] = updateAnimData(nebulae[i], dT, 7);
    }

    EndDrawing();
  }
  UnloadTexture(background);
  UnloadTexture(scarfy);
  UnloadTexture(nebula);
  CloseWindow();
}

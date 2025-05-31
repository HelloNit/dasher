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

  const int gravity{1'000}; //(pixel / frame)

  Texture2D nebula = LoadTexture("texture/12_nebula_spritesheet.png");
  Rectangle nebRec{0.0, 0.0, nebula.width / 8.0f, nebula.height / 8.0f};
  Vector2 nebPos{windowWidth, windowHeight - nebRec.height};

  Rectangle neb2Rec{0.0, 0.0, nebula.width / 8.0f, nebula.height / 8.0f};
  Vector2 neb2Pos{windowWidth + 300, windowHeight - nebRec.height};

  // animação da neblina
  int nebFrame{};
  const float nebUpdateTime{1.0 / 12.0};
  float nebRunningTime{};

  int neb2Frame{};
  const float neb2UpdateTime{1.0 / 16.0};
  float neb2RunningTime{};

  // velocidade da neblina
  int nebVel{-600};

  Texture2D scarfy = LoadTexture("texture/scarfy.png");
  AnimData scarfyData; 
  scarfyData.rec.width = scarfy.width / 6;
  scarfyData.rec.height = scarfy.height;
  scarfyData.rec.x = 0;
  scarfyData.rec.y = 0;
  scarfyData.pos.x = windowWidth / 2 - scarfyData.rec.width / 2;
  scarfyData.pos.y = windowHeight - scarfyData.rec.height;
  scarfyData.frame = 0;
  scarfyData.updateTime = 1.0 / 12.0;
  scarfyData.runningTime = 0.0;

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
  float runningTime{};

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

    // atualiza a posição da neblina e fé
    nebPos.x += nebVel * dT;

    // atualiza a posição da neblina jr e fé
    neb2Pos.x += nebVel * dT;

    // atualiza a posição do boneco
    scarfyPos.y += velocity * dT;

    if (isInAir == false)
    {
      runningTime += dT;
      if (runningTime >= updateTime)
      {
        runningTime = 0;
        scarfyRec.x = frame * scarfyRec.width;
        frame++;
        if (frame > 24)
        {
          frame = 0;
        }
      }
    }

    // atualiza o running time
    runningTime += dT;
    if (runningTime >= updateTime)
    {
      runningTime = 0;
      scarfyRec.x = frame * scarfyRec.width;
      frame++;
      if (frame > 24)
      {
        frame = 0;
      }
    }

    // atualiza o running time da neblina
    nebRunningTime += dT;
    if (nebRunningTime >= nebUpdateTime)
    {
      nebRunningTime = 0.0;
      nebRec.x = nebFrame * nebRec.width;
      nebFrame++;
      if (nebFrame > 8)
      {
        nebFrame = 0;
      }
    }

    // atualiza o running time da neblina jr
    neb2RunningTime += dT;
    if (neb2RunningTime >= neb2UpdateTime)
    {
      neb2RunningTime = 0.0;
      neb2Rec.x = neb2Frame * neb2Rec.width;
      neb2Frame++;
      if (neb2Frame > 8)
      {
        neb2Frame = 0;
      }
    }

    // neblina
    DrawTextureRec(nebula, nebRec, nebPos, WHITE);

    // neblina jr
    DrawTextureRec(nebula, neb2Rec, neb2Pos, ORANGE);

    // personagem
    DrawTextureRec(scarfy, scarfyRec, scarfyPos, WHITE);

    // final do código de bosta
    EndDrawing();
  }
  UnloadTexture(scarfy);
  UnloadTexture(nebula);
  CloseWindow();
}
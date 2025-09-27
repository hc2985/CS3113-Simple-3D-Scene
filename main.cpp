
/**
* Author: Hyeonung Cho
* Assignment: Simple 2D Scene
* Date due: 2025-09-27, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

    #include "CS3113/cs3113.h"
    #include <math.h>

    // Global Constants
    constexpr int   SCREEN_WIDTH  = 1600,
                    SCREEN_HEIGHT = 900,
                    FPS           = 60,
                    SIZE          = 600,
                    SMALL         = 300;
    constexpr float MAX_AMP       = 100.0f;

    constexpr char    BG_COLOUR[] = "#B2AAC6";
    constexpr Vector2 ORIGIN      = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
    constexpr Vector2 L_ORIGIN      = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2 };
    constexpr Vector2 R_ORIGIN      = { SCREEN_WIDTH/1.2, SCREEN_HEIGHT / 2 };
    constexpr Vector2 BASE_SIZE   = { (float) SIZE, (float) SIZE };
    constexpr Vector2 THIN_SIZE   = { (float) SMALL, (float) SIZE};

    // Images
    constexpr char gun_fp[]  = "gun.png";
    constexpr char ronaldo_fp[] = "ronaldo.png";
    constexpr char rockman_fp[] = "rockman.jpg";
    constexpr char background_fp[] = "background.jpg";

    // Global Variables
    AppStatus gAppStatus     = RUNNING;
    float     gScaleFactor   = SIZE,
            gAngle         = 0.0f,
            gAngle2        = 0.0f,
            gPulseTime     = 0.0f,
            gPulseTime_slow= 0.0f;
    Vector2   gPosition      = ORIGIN;
    Vector2   gScale         = BASE_SIZE;
    Vector2   gScale2        = BASE_SIZE;
    Vector2   L_Trans        = {SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2};
    Vector2   R_Trans        = { SCREEN_WIDTH/1.2, SCREEN_HEIGHT / 2 };
    Vector2   Mix_Trans      = {0, 0};
    float     gPreviousTicks = 0.0f;

    Texture2D gun;
    Texture2D ronaldo;
    Texture2D rockman;
    Texture2D background;

    // Function Declarations
    void initialise();
    void processInput();
    void update();
    void render();
    void shutdown();

    // Function Definitions
    void initialise()
    {
        InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Textures");

        gun = LoadTexture(gun_fp);
        ronaldo = LoadTexture(ronaldo_fp);
        rockman = LoadTexture(rockman_fp);
        background = LoadTexture(background_fp);

        SetTargetFPS(FPS);
    }

    void processInput() 
    {
        if (WindowShouldClose()) gAppStatus = TERMINATED;
    }

    void update() 
    {
        // Delta time
        float ticks = (float) GetTime();
        float deltaTime = ticks - gPreviousTicks;
        gPreviousTicks = ticks;

        gPulseTime += 10.0f * deltaTime;
        gPulseTime_slow += 3.0f * deltaTime;


        gScale2 = {
            THIN_SIZE.x + MAX_AMP * cos(gPulseTime),
            THIN_SIZE.y + MAX_AMP * cos(gPulseTime)
        };

        gScale = {
            BASE_SIZE.x + MAX_AMP * cos(gPulseTime),
            BASE_SIZE.y + MAX_AMP * cos(gPulseTime)
        };

        L_Trans = {
            L_ORIGIN.x + 300.0f * cos(gPulseTime_slow),
            L_ORIGIN.y + 300.0f * cos(gPulseTime_slow+gPulseTime)
        };

        R_Trans = {
            R_ORIGIN.x + 500.0f * sin(gPulseTime_slow),
            R_ORIGIN.y + 500.0f * sin(gPulseTime_slow)
        };

        Mix_Trans = {
            R_ORIGIN.x + 500.0f * sin(gPulseTime),
            L_ORIGIN.x + 300.0f * cos(gPulseTime_slow)
        };

        gAngle2 += 30.0f * deltaTime; //rotation
    }

    void render()
    {
        BeginDrawing();
        ClearBackground(WHITE);

        //background 
        Rectangle background_area = {
            0.0f, 0.0f,
            static_cast<float>(background.width),
            static_cast<float>(background.height)
        };

        // gun texture (UV coordinates)
        Rectangle gun_area = {
            // top-left corner
            0.0f, 0.0f,
            // bottom-right corner (of texture)
            static_cast<float>(gun.width),
            static_cast<float>(gun.height)
        };

        // ronaldo texture (UV coordinates)
        Rectangle ronaldo_area = {
            // top-left corner
            0.0f, 0.0f,
            // bottom-right corner (of texture)
            static_cast<float>(ronaldo.width),
            static_cast<float>(ronaldo.height)
        };

        // gun texture (UV coordinates)
        Rectangle rockman_area = {
            // top-left corner
            0.0f, 0.0f,
            // bottom-right corner (of texture)
            static_cast<float>(rockman.width),
            static_cast<float>(rockman.height)
        };

        // Where background is
        Rectangle backgroundArea = {
            gPosition.x,    
            gPosition.y,
            static_cast<float>(SCREEN_WIDTH),
            static_cast<float>(SCREEN_HEIGHT)
        };

        //middle - for gun
        Rectangle originalArea = {
            Mix_Trans.x,    
            Mix_Trans.y,
            static_cast<float>(gScale.x),
            static_cast<float>(gScale.y)
        };

        //left - for ronaldo
        Rectangle leftArea = {
            L_Trans.x,    
            L_Trans.y,
            static_cast<float>(gScale2.x),
            static_cast<float>(gScale2.y)
        };

        //right - for rockman
        Rectangle rightArea = {
            R_Trans.x,    
            R_Trans.y,
            static_cast<float>(gScale.x),
            static_cast<float>(gScale.y)
        };

        //center
        Vector2 originalOrigin = {
            static_cast<float>(gScale.x) / 2.0f,
            static_cast<float>(gScale.y) / 2.0f
        };
    

        // Origin inside the source texture (centre of the texture)
        Vector2 objectOrigin = {
            static_cast<float>(SCREEN_WIDTH) / 2.0f,
            static_cast<float>(SCREEN_HEIGHT) / 2.0f
        };

        // Render the texture on screen
        DrawTexturePro(
            background,
            background_area,  
            backgroundArea, 
            objectOrigin, 
            gAngle,
            WHITE
        );

        DrawTexturePro(
            gun,
            gun_area,
            originalArea,
            originalOrigin,
            gAngle,
            WHITE
        );

        DrawTexturePro(
            ronaldo,
            ronaldo_area,
            leftArea,
            originalOrigin,
            gAngle,
            WHITE
        );

        DrawTexturePro(
            rockman,
            rockman_area,
            rightArea,
            originalOrigin,
            gAngle2,
            WHITE
        );

        

        EndDrawing();
    }

    void shutdown() { CloseWindow(); }

    int main(void)
    {
        initialise();

        while (gAppStatus == RUNNING)
        {
            processInput();
            update();
            render();
        }

        shutdown();

        return 0;
    }
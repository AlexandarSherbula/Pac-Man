#include "Game.h"

Game::Game()
{
    sAppName = "Pac-Man";
}

bool Game::OnUserCreate()
{
    ImageAssets::get().LoadSprites();

    player = std::make_unique<Player>(this);
    player->SetSprite("Pacman", true);
    player->SetMovement(olc::vf2d(15.5f, 23.0f), 7.0f, olc::vi2d(-1, 0));
    
    ghosts[0] = std::make_unique<Ghost>(this, GhostHouse::OUTSIDE, 0);
    ghosts[0]->SetMovement(olc::vf2d(15.5f, 11.0f), 7.0f, olc::vi2d(-1, 0));
    ghosts[0]->SetTileID(olc::vi2d(0, 0));

    ghosts[1] = std::make_unique<Ghost>(this, GhostHouse::GOING_UP, 0);    
    ghosts[1]->SetMovement(olc::vf2d(15.5f, 14.0f), 7.0f, olc::vi2d(0, -1));
    ghosts[1]->SetTileID(olc::vi2d(6, 1));

    ghosts[2] = std::make_unique<Ghost>(this, GhostHouse::INSIDE, 30);
    ghosts[2]->SetMovement(olc::vf2d(13.5f, 14.0f), 7.0f, olc::vi2d(0, -1));
    ghosts[2]->SetTileID(olc::vi2d(4, 2));

    ghosts[3] = std::make_unique<Ghost>(this, GhostHouse::INSIDE, 60);
    ghosts[3]->SetMovement(olc::vf2d(17.5f, 14.0f), 7.0f, olc::vi2d(0, -1));
    ghosts[3]->SetTileID(olc::vi2d(4, 3));

    for (int i = 0; i < ghosts.size(); i++)
    {
        ghosts[i]->SetSprite("Ghost", true);        
        ghosts[i]->CheckHouseMode();        
    }    

    level = std::make_unique<Level>(this);
    level->SetSprite("Wall", true);

    pacmanDying = std::make_unique<Object>(this);
    pacmanDying->SetSprite("DeadPacman", true);

    wait = 0.0f;

    isDead = false;
    ghostHit = false;
    pause = false;
    return true;
}

bool Game::OnUserUpdate(float fElapsedTime)
{
    if (GetKey(olc::SPACE).bPressed) pause = !pause;

    if (!pause)
    {
        wait += fElapsedTime;

        if (wait > 4.5f)
        {
            if (!ghostHit)
            {
                // Collect power Pellets
                if (level->GetTile(player->position) == '*' ||
                    level->GetTile(player->position) == '0' ||
                    level->GetTile(player->position) == 'x' ||
                    level->GetTile(player->position) == 'X')
                {
                    if (level->GetTile(player->position) == '*' ||
                        level->GetTile(player->position) == 'X')
                    {
                        for (int i = 0; i < ghosts.size(); i++)
                        {
                            if (!player->energized)
                                ghosts[i]->lastMode = ghosts[i]->mode;
                            ghosts[i]->InitFright = true;
                        }

                        player->energized = true;
                        player->energizedTimer = 0.0f;
                    }

                    level->SetTile(player->position, '_');
                    player->pelletCounter++;
                    player->noPelletTimer = 0.0f;
                }

                player->Update(fElapsedTime, level.get());

                for (int i = 0; i < ghosts.size(); i++)
                {
                    ghosts[i]->Update(fElapsedTime, level.get(), player.get(), i);
                }

                level->Update(fElapsedTime);

                for (int i = 0; i < ghosts.size(); i++)
                {
                    if ((cmp(player->position, ghosts[i]->position, 0.5f) ||
                        cmp(ghosts[i]->position, player->position, 0.5f))
                        && !ghosts[i]->eaten)
                    {
                        if (player->energized)
                        {
                            wait = 3.5f;
                            ghosts[i]->eaten = true;
                        }
                        else
                        {
                            ghostHit = true;
                            wait = 3.0f;
                        }
                    }
                        
                }
            }
            else
                isDead = true;
        }

        if (pacmanDying->tileID.x == 13)
        {
            if (wait > 0.5f)
            {
                pacmanDying->tileID = { 0, 0 };
                player->Restart();
                for (int i = 0; i < ghosts.size(); i++)
                    ghosts[i]->Restart();
                wait = 2.5f;
                ghostHit = false;
                isDead = false;
            }
        }
    }
    Clear(olc::BLACK);

    if (!isDead)
    {
        std::cout << player->tileID << std::endl;
        player->DrawPartial({ 168.0f + player->position.x * 16.0f, 32.0f + player->position.y * 16.0f }, olc::vi2d(32, 32));
        for (int i = 0; i < ghosts.size(); i++)
            ghosts[i]->DrawPartial({ 168.0f + ghosts[i]->position.x * 16.0f, 32.0f + ghosts[i]->position.y * 16.0f }, olc::vi2d(32, 32));
    }

    //Pacman's dying animation
    if (isDead && pacmanDying->tileID.x != 13)
    {
        if (wait >= 0.1f)
        {
            pacmanDying->Animate(0, 13, fElapsedTime, 0.0f);
            wait = 0.0f;
        }
        pacmanDying->DrawPartial({ 168.0f + player->position.x * 16.0f, 32.0f + player->position.y * 16.0f }, { 32, 32 });
    }

    level->Draw();

    return !GetKey(olc::ESCAPE).bPressed;
}

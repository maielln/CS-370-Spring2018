	while (cnta < 10) {
		while (cnt < 200) {
			spritePos[cycle].x++;

			if (spritePos[cycle].x >= 589) {
				cout << "break" << endl;
				cout <<  spritePos[cycle].x << endl;
				break;
			}

			SDL_BlitSurface(sprites[rob1], NULL, aWindowSurface,  &spritePos[cycle]);
			SDL_UpdateWindowSurface(aWindow);
			cnt++;
		}

		while (cnt > -500) {
			spritePos[cycle].x--;

			if (spritePos[cycle].x <= 51) {
				cout << "break" << endl;
				cout <<  spritePos[cycle].x << endl;
				break;
			}

			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, rob1T, NULL, &spritePos[cycle]);

            SDL_RenderPresent(renderer);
			//SDL_UpdateWindowSurface(aWindow);
			cnt--;
		}

		cnta++;

	}

	
	
	
	
	cnt = 0;
	cnta = 0;
    cout << "y pos is " << spritePos[cycle].y << endl;
    while (cnta < 5) {
        while (cnt > -200) {
			spritePos[cycle].y--;
			//cout <<  spritePos[cycle].x << endl;

			if (spritePos[cycle].y <= 51) {
				cout << "break" << endl;
				cout <<  spritePos[cycle].y << endl;
				break;
			}

			SDL_BlitSurface(robot2, NULL, aWindowSurface,  &spritePos[cycle]);
			SDL_UpdateWindowSurface(aWindow);
			cnt--;
		}

		while (cnt < 1000) {
			spritePos[cycle].y++;
			//cout <<  spritePos[cycle].x << endl;

			if (spritePos[cycle].y >= 539) {
				cout << "break" << endl;
				cout <<  spritePos[cycle].y << endl;
				break;
			}
			SDL_BlitSurface(robot2, NULL, aWindowSurface,  &spritePos[cycle]);
			SDL_UpdateWindowSurface(aWindow);
			cnt++;
		}

		cnta++;

    }

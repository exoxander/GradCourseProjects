/*============================================================
		Alexander Lustig
		CS6420
		Assignment 6: Exploring ant colony optimization
		Dr. Rague
		Due April 23 2023

		This file uses the OLC Pixel Game Engine to visualize my ant colony optimization algorithm
		operating on a randomly generated graph

		I wasnt able to get command line arguments to pass into the engine correctly so
		the variables to change the program start on line 29.
============================================================*/

#define OLC_PGE_APPLICATION
#include "Ant.h"
#include <iostream>
#include <string>
#include "Button.h"
#include "olcPixelGameEngine.h"

class Visualizer : public olc::PixelGameEngine {
private:

	//==============================================================================================
	//unfortunatley i was unable to get command-line arguments working correctly for this program
	//these three variables control the number of vertecies in the graph, the total number of trials
	//for the ants, and the number of trials for each frame, all three of these can be adjusted to
	//increase the programs performance if need be
	int graphSize = 100;
	int antTrials = 20000;
	int trialsPerFrame = 10;

	//this enables or disables showing the pheramone paths during the trials
	bool showPheramonePath = false;
	//==============================================================================================

	int antProgression = antTrials;
	float minDistance = 0;
	float lastDistance = 0;

	std::vector<int> bestPath;

	Graph graph = Graph(graphSize);
	Ant ant;

	Button RetryButton = Button(10, 40, 140, 40, "Run Trials");
	Button ResetButton = Button(10, 100, 140, 40, "New Graph");
	Button PathsButton = Button(10, 160, 140, 40, "Pheramone Paths");


	//do not change without cause, its like tuning a piano where each
	//string is both mislabeled and goes to the wrong place

	float distanceWeight = 4;//4
	float pheramoneWeight = 1;//1

	float pheramoneAmp = 10;//10
	float evaporationAmp = .1;//0.1

public:
	Visualizer() {
		sAppName = "Ant Colony Visualizer";
	}
	bool OnUserCreate() {
		srand((std::chrono::duration_cast<std::chrono::nanoseconds> (std::chrono::system_clock::now().time_since_epoch()).count()));

		//create graph with random vertecies
		graph.generate(ScreenWidth(), ScreenHeight());

		//initialize the ant
		ant = Ant(&graph, pheramoneWeight, distanceWeight);


		//do not change these without cause, its like tuning a piano where each
		//string is mislabeled and goes to the wrong place

		//ant decision variables
		//changes how the ant decides based on distance and pheramones
		float distanceWeight = 2;//2
		float pheramoneWeight = 8;//10

		//pheramone valiables
		//controls how long the ants will try to follow the same path
		float pheramoneAmp = 200;//50
		float evaporationAmp = 1;//1

		return true;
	}
	bool OnUserUpdate(float fElapsedTime) override {

		//run [trialsPerFrame] ants per update until all ants done
		int t = 0;
		while (antProgression < antTrials && t < trialsPerFrame) {

			//The ants go marching
			//move the ant through the entire graph
			for (int v = 0; v < graph.size; v++) {
				ant.move();
			}

			//check if this ants path is shorter than the previous one and put a trail on it
			if (ant.totalDistance < lastDistance) {
				//do a small pheramone placement
				graph.applyPheramones(ant.path, pheramoneAmp);
			}

			//check if this path is a new minimum distance and apply a huge pheramone boost
			if (antProgression == 0 || ant.totalDistance < minDistance) {
				bestPath = ant.path;
				minDistance = ant.totalDistance;
				lastDistance = minDistance;
				//massive pheramone addition to this trail
				graph.applyPheramones(ant.path, pheramoneAmp * pheramoneAmp);
			}

			//set the last distance, evaporate pheramones, reset ant for next run
			lastDistance = ant.totalDistance;
			graph.evaporate(evaporationAmp);
			ant.reset();

			//global ant trials tracker
			antProgression++;
			//per update trials tracker
			t++;
		}

		//fill screen
		for (int x = 0; x < ScreenWidth(); x++)
			for (int y = 0; y < ScreenHeight(); y++)
				Draw(x, y, olc::Pixel(50, 60, 80));

		//-----------------------------< Buttons >----------------------------

		//run trials
		FillRect(RetryButton.x, RetryButton.y, RetryButton.xSize, RetryButton.ySize, olc::WHITE);
		DrawString(RetryButton.x + 5, RetryButton.y + 5, RetryButton.name, olc::BLACK);

		//do button action
		if (GetMouse(0).bPressed && RetryButton.clicked(GetMouseX(), GetMouseY())) {

			//set a trial going
			antProgression = 0;
			graph.reset();
		}

		//new graph
		//color button
		FillRect(ResetButton.x, ResetButton.y, ResetButton.xSize, ResetButton.ySize, olc::WHITE);
		DrawString(ResetButton.x + 5, ResetButton.y + 5, ResetButton.name, olc::BLACK);

		//do button action
		if (GetMouse(0).bPressed && ResetButton.clicked(GetMouseX(), GetMouseY())) {

			//make new graph and ready for trials
			antProgression = antTrials;
			bestPath.clear();
			minDistance = 0;
			graph.generate(ScreenWidth(), ScreenHeight());
		}

		//show/hide pheramone trails
		FillRect(PathsButton.x, PathsButton.y, PathsButton.xSize, PathsButton.ySize, olc::WHITE);
		DrawString(PathsButton.x + 5, PathsButton.y + 5, PathsButton.name, olc::BLACK);

		//do button action
		if (GetMouse(0).bPressed && PathsButton.clicked(GetMouseX(), GetMouseY())) {
			showPheramonePath = !showPheramonePath;
		}

		//-----------------------------< Buttons >----------------------------

		//render edges, shows the ants bets path and optionally the pheramone paths
		renderEdges();

		//renders the graph vertecies
		renderVertecies();

		//show progress and best path length
		DrawString(20, 10, "Ant Progression: " + std::to_string(antProgression) + " / " + std::to_string(antTrials));
		DrawString(20, 20, "Shortest path found: " + std::to_string(minDistance));

		return true;
	}

	//show all graph vertecies, coloring the start / end as red
	void renderVertecies() {
		//draw vertecies
		for (int v = 0; v < graph.size; v++) {
			olc::Pixel color = olc::GREEN;
			if (v == 0) {
				color = olc::RED;
			}
			FillCircle(graph.vertecies[v].x, graph.vertecies[v].y, 4, color);
		}
	}

	//show the best path the ants have found
	//optionally show high taffic pheramone paths
	void renderEdges() {

		//pheramone paths
		if (showPheramonePath && antProgression < antTrials) {
			for (int v = 0; v < graph.size; v++) {
				for (int e = 0; e < graph.size; e++) {
					if (v != e) {
						//source position
						float sx = graph.vertecies[v].x;
						float sy = graph.vertecies[v].y;

						//end position
						float ex = graph.vertecies[e].x;
						float ey = graph.vertecies[e].y;
						int a = int(graph.edges[graph.size * v + e].pheramoneStrength * 10) % 255;
						if (a > 90) {
							DrawLine(sx, sy, ex, ey, olc::Pixel(255, 255, 255, a));
						}
					}
				}
			}
		}

		//draw best path the ants have found
		//edge = [size * i-1 + i]
		for (int i = 1; i <= bestPath.size(); i++) {
			//source position
			float sx = graph.vertecies[bestPath[i - 1]].x;
			float sy = graph.vertecies[bestPath[i - 1]].y;

			float ex;
			float ey;
			if (i != bestPath.size()) {
				//end position
				ex = graph.vertecies[bestPath[i]].x;
				ey = graph.vertecies[bestPath[i]].y;
			}
			else {
				//link to final (start) vertex
				ex = graph.vertecies[bestPath[0]].x;
				ey = graph.vertecies[bestPath[0]].y;
			}

			DrawLine(sx, sy, ex, ey, olc::CYAN);
		}
	}
};

int main() {

	Visualizer v;
	if (v.Construct(1280, 720, 1, 1, false, true)) {

		v.Start();
	}
	return 0;
}
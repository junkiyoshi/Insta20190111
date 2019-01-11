#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetLineWidth(1.5);

	this->fbo.allocate(ofGetWidth(), ofGetHeight());

	this->number_of_agent = 200;
	this->len_of_tail = 10;
}

//--------------------------------------------------------------
void ofApp::update() {

	this->vertices.clear();

	this->fbo.begin();
	ofClear(0);

	float radius = 350;
	for (int deg = 0; deg < 360; deg += 3) {

		glm::vec2 noise_point = glm::vec2(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD));
		float noise_radius = ofMap(ofNoise(noise_point.x * 0.005, noise_point.y * 0.005, ofGetFrameNum() * 0.005), 0, 1, radius * 0.3, radius);

		this->vertices.push_back(glm::vec2(noise_radius * cos(deg * DEG_TO_RAD) + ofGetWidth() * 0.5, noise_radius * sin(deg * DEG_TO_RAD) + ofGetHeight() * 0.5));
	}

	ofFill();
	ofBeginShape();
	ofVertices(this->vertices);
	ofEndShape(true);

	this->fbo.end();
	this->fbo.readToPixels(this->pixels);

	while (this->logs.size() < this->number_of_agent) {

		int x = ofRandom(ofGetWidth());
		int y = ofRandom(ofGetHeight());
		if (this->pixels.getColor(x, y) != ofColor(0, 0)) {

			deque<glm::vec2> log;
			log.push_front(glm::vec2(x, y));
			this->logs.push_back(log);
			this->directions.push_back(glm::normalize(glm::vec2(ofRandom(-1, 1), ofRandom(-1, 1))));
			ofColor color;
			color.setHsb(ofRandom(255), 255, 200);
			this->colors.push_back(color);
		}
	}

	for (int i = 0; i < this->number_of_agent; i++) {

		glm::vec2 future = this->logs[i].front() + this->directions[i] * 5;
		int x = future.x;
		int y = future.y;

		while (this->pixels.getColor(x, y) == ofColor(0, 0)) {

			this->directions[i] = glm::normalize(glm::vec2(ofRandom(-1, 1), ofRandom(-1, 1)));

			future = this->logs[i].front() + this->directions[i] * 10;
			x = future.x;
			y = future.y;
		}

		this->logs[i].push_front(future);
		while (this->logs[i].size() > this->len_of_tail) {

			this->logs[i].pop_back();
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofNoFill();
	ofSetColor(39);
	ofBeginShape();
	ofVertices(this->vertices);
	ofEndShape(true);

	for (int i = 0; i < this->number_of_agent; i++) {

		for (int l = 0; l < this->logs[i].size() - 1; l++) {

			ofSetColor(this->colors[i], ofMap(l, 0, this->len_of_tail, 255, 64));
			ofDrawLine(this->logs[i][l], this->logs[i][l + 1]);
		}
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}
# include "my_viewer.h"

# include <sigogl/ui_button.h>
# include <sigogl/ui_radio_button.h>
# include <sig/sn_primitive.h>
# include <sig/sn_transform.h>
# include <sig/sn_manipulator.h>
# include <sig/sn_node.h>

# include <sigogl/ws_run.h>


float dangle = 0.03f;
float handAngle = 0;
float lowArmAngle = 0;
float lowArmDangle = 0.05f;
float upArmAngle = 0;
float upArmDangle = 0.07f;
bool animate = false;

MyViewer::MyViewer(int x, int y, int w, int h, const char* l) : WsViewer(x, y, w, h, l)
{
	_nbut = 0;
	_animating = false;
	build_ui();
	build_scene();
}

void MyViewer::build_ui()
{
	UiPanel *p, *sp;
	UiManager* uim = WsWindow::uim();
	p = uim->add_panel("", UiPanel::HorizLeft);
	p->add(new UiButton("View", sp = new UiPanel()));
	{	UiPanel* p = sp;
	p->add(_nbut = new UiCheckButton("Normals", EvNormals));
	}
	p->add(new UiButton("Animate", EvAnimate));
	p->add(new UiButton("Exit", EvExit)); p->top()->separate();
}

void MyViewer::add_model(SnShape* s, GsVec p)
{
	SnManipulator* manip = new SnManipulator;
	GsMat m;
	m.translation(p);
	manip->initial_mat(m);

	SnGroup* g = new SnGroup;
	SnLines* l = new SnLines;
	l->color(GsColor::orange);
	g->add(s);
	g->add(l);
	manip->child(g);

	rootg()->add(manip);
}

void MyViewer::build_scene()
{
	SnGroup* mainArm_group = new SnGroup; //Contains all arm components

	GsMat m;
	GsMat roty, rotz;
	roty.roty(3.1415f / 2);
	rotz.rotz(3.1415f / 2);
	m.translation(GsVec(-30, 0, 0));
	m.mult(m, roty);
	m.mult(m, rotz);
	upArm_manip->initial_mat(m);

	SnModel* upArm = new SnModel;
	upArm->model()->load("C:\\Users\\Veronica\\Documents\\CSE170\\PA4\\arm\\rupperarm.m");
	upArm->model()->smooth();

	upArm_manip->child(mainArm_group);
	mainArm_group->add(upArm);

	GsMat lA;
	lA.translation(GsVec(0, 0, 25));
	lowArm_manip->initial_mat(lA);

	SnGroup* lowArm_group = new SnGroup;
	SnModel* lowArm = new SnModel;
	lowArm->model()->load("C:\\Users\\Veronica\\Documents\\CSE170\\PA4\\arm\\rlowerarm.m");
	lowArm->model()->smooth();

	mainArm_group->add(lowArm_manip);
	lowArm_manip->child(lowArm_group);
	lowArm_group->add(lowArm);

	GsMat handMat;
	handMat.translation(GsVec(0, 0, 25));
	hand_manip->initial_mat(handMat);

	SnGroup* hand_group = new SnGroup;
	SnModel* hand = new SnModel;
	hand->model()->load("C:\\Users\\Veronica\\Documents\\CSE170\\PA4\\arm\\rhand.m");
	hand->model()->smooth();

	lowArm_group->add(hand_manip);
	hand_manip->child(hand_group);
	hand_group->add(hand);

	rootg()->add(upArm_manip);
}


void MyViewer::rotation(int type){
	GsMat wholeArm_mat = upArm_manip->mat();
	GsMat lowarm_mat = lowArm_manip->mat();
	GsMat hand_mat = hand_manip->mat();



	if (type == 0 && animate) { //hand moves in the positive y angle
		GsMat rotation;
		
		
		rotation.roty(dangle);

		hand_mat.mult(hand_mat, rotation);
		hand_manip->initial_mat(hand_mat);

		render(); // notify it needs redraw
		ws_check(); // redraw now
	}

	if (type == 1 && animate) { //hand moves in the negative y angle
		GsMat rotation;


		rotation.roty(-dangle);

		hand_mat.mult(hand_mat, rotation);
		hand_manip->initial_mat(hand_mat);

		render(); // notify it needs redraw
		ws_check(); // redraw now
	}
	if (type == 2 && animate) { //low arm moves in the positive y angle
		GsMat rotation;


		rotation.roty(lowArmDangle);

		lowarm_mat.mult(lowarm_mat, rotation);
		lowArm_manip->initial_mat(lowarm_mat);

		render(); // notify it needs redraw
		ws_check(); // redraw now
	}
	if (type == 3 && animate) { //low arm moves in the negative y angle
		GsMat rotation;


		rotation.roty(-lowArmDangle);

		lowarm_mat.mult(lowarm_mat, rotation);
		lowArm_manip->initial_mat(lowarm_mat);

		render(); // notify it needs redraw
		ws_check(); // redraw now
	}

	if (type == 4 && animate) { //hand moves in the positive y angle
		GsMat rotation;


		rotation.roty(upArmDangle);

		wholeArm_mat.mult(wholeArm_mat, rotation);
		upArm_manip->initial_mat(wholeArm_mat);

		render(); // notify it needs redraw
		ws_check(); // redraw now
	}

	if (type == 5 && animate) { //hand moves in the negative y angle
		GsMat rotation;


		rotation.roty(-upArmDangle);

		wholeArm_mat.mult(wholeArm_mat, rotation);
		upArm_manip->initial_mat(wholeArm_mat);

		render(); // notify it needs redraw
		ws_check(); // redraw now
	}
}

// Below is an example of how to control the main loop of an animation:
void MyViewer::run_animation()
{
	double frdt = 1.0/30.0; // delta time to reach given number of frames per second
	double t=0, lt=0, t0=gs_time();
	
	do {
		//while (t - lt<frdt) { t = gs_time() - t0; }
		//lt = t;
		while (t < 2) {

			while (upArmAngle <= (1.57f)) {
				animate = true;
				rotation(4);
				upArmAngle += upArmDangle;
			}
			while (lowArmAngle <= (2.09f)) {
				animate = true;
				rotation(2);
				lowArmAngle += lowArmDangle;
			}

			while (handAngle <= (1.57f)) {
				animate = true;
				rotation(0);
				handAngle += dangle;
			}
			t += 0.1f;
			//t = 3;
		}
		t = 0;
		while (t<1000000){
			t += 0.1f;
			rotation(9);
		}
		t = 0;
		while (t < 2) {
			while (upArmAngle >= -(1.57f)) {
				animate = true;
				rotation(5);
				upArmAngle -= upArmDangle;
			}
			while (lowArmAngle >= (0)) {
				animate = true;
				rotation(3);
				lowArmAngle -= lowArmDangle;
			}

			while (handAngle >= -(1.57f)) {
				animate = true;
				rotation(1);
				handAngle -= dangle;
			}
			t += 0.1f;
		}

		t = 0;
		while (t < 1000000) {
			t += 0.1f;
			rotation(9);
		}

		t = 0;
		while (t < 2) {

			while (upArmAngle <= 0) {
				animate = true;
				rotation(4);
				upArmAngle += upArmDangle;
			}

			while (handAngle <= (0)) {
				animate = true;
				rotation(0);
				handAngle += dangle;
			}
			t += 0.1f;
			//t = 3;
		}

			
		
	} while (_animating);
	

	
		
	
}

void MyViewer::show_normals(bool b) {

	GsArray<GsVec> fn;
	SnGroup* r = (SnGroup*)root();
	for (int k = 0; k<r->size(); k++)
	{
		SnManipulator* manip = r->get<SnManipulator>(k);
		SnShape* s = manip->child<SnGroup>()->get<SnShape>(0);
		SnLines* l = manip->child<SnGroup>()->get<SnLines>(1);
		if (!b) { l->visible(false); continue; }
		l->visible(true);
		if (!l->empty()) continue; // build only once
		l->init();
		if (s->instance_name() == SnPrimitive::class_name)
		{
			GsModel& m = *((SnModel*)s)->model();
			m.get_normals_per_face(fn);
			const GsVec* n = fn.pt();
			float f = 0.33f;
			for (int i = 0; i<m.F.size(); i++)
			{
				const GsVec& a = m.V[m.F[i].a]; l->push(a, a + (*n++)*f);
				const GsVec& b = m.V[m.F[i].b]; l->push(b, b + (*n++)*f);
				const GsVec& c = m.V[m.F[i].c]; l->push(c, c + (*n++)*f);
			}
		}
	}
}

int MyViewer::handle_keyboard(const GsEvent &e)
{
	int ret = WsViewer::handle_keyboard(e); // 1st let system check events
	if (ret) return ret;

	switch (e.key)
	{
	case GsEvent::KeyEsc: gs_exit(); return 1;
	case 'n': {
		bool b = !_nbut->value(); _nbut->value(b); show_normals(b); return 1;
	}
	case 'e': { //move hand up
		
		if (handAngle >= (1.57f)) {
			//animate = false;
			return 1;
		}
		else {
			animate = true;
			rotation(0);
			handAngle += dangle;
			return 1;
		}
		return 1;
	}

	case 'd': { //move hand down
		if (handAngle <= -(1.57f)) {
			animate = false;
			return 1;
		}
		else {
			animate = true;
			rotation(1);
			handAngle -= dangle;
			return 1;
		}
		return 1;
	}
	case 'w': { //move lower arm up
		if (lowArmAngle >= (2.09f)) {
			//animate = false;
			return 1;
		}
		else {
			animate = true;
			rotation(2);
			lowArmAngle += lowArmDangle;
			return 1;
		}
		return 1;
	}
	case 's': { //move lower arm down
		if (lowArmAngle <= 0) {
			//animate = false;
			return 1;
		}
		else {
			animate = true;
			rotation(3);
			lowArmAngle -= lowArmDangle;
			return 1;
		}
		return 1;
	}

	case 'q': { //move upper arm up
		if (upArmAngle >= (1.57f)) {
			return 1;
		}
		else {
			animate = true;
			rotation(4);
			upArmAngle += upArmDangle;
			return 1;
		}
	}
	
	case 'a': {
		if (upArmAngle <= -(1.57f)) {
			return 1;
		}
		else {
			animate = true;
			rotation(5);
			upArmAngle -= upArmDangle;
			return 1;
		}
	}

	default: gsout << "Key pressed: " << e.key << gsnl;
	}

	return 0;
}

int MyViewer::uievent(int e)
{
	switch (e)
	{
	case EvNormals: show_normals(_nbut->value()); return 1;
	case EvAnimate: run_animation(); return 1;
	case EvExit: gs_exit();
	}
	return WsViewer::uievent(e);
}

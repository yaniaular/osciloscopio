#ifndef UTILIDADES_H_
#define UTILIDADES_H_
	typedef float Vector [4];
	int ValAbs(float val);
	int rot(int a, int mod);
	float snap(float val, float grid);
	float afloor_snap(float val, float grid);
	float min(float a, float b);
	float max(float a, float b);
	void doVertex(Vector v);
	float timeDiff(timeval a, timeval b);
	float truncar(float num);
	float comparar(float x1, float x2);
#endif

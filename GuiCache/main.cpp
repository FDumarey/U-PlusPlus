#include <CtrlLib/CtrlLib.h>

using namespace Upp;

Image CreateBall (int r, Color color)
{
	int rr = 2*r;
	int r2 = r*r;
	ImageBuffer b(rr,rr);
	for(int y=0;y<rr;y++)
		for(int x=0;x<rr;x++) {
			RGBA& a = b[y][x];
			a.r = color.GetR();
			a.g = color.GetG();
			a.b = color.GetB();
			int q=((x-r)*(x-r)+(y-r)*(y-r))*256/r2;
			a.a = q <= 255 ? q:0;
		}
		Premultiply(b);
		return b;
}

struct BallMaker : ImageMaker { // ImageMaker is cached version
	Color color;
	int r;
	virtual String Key() const; // Key = unique String
	virtual Image Make() const; // Make = Image to be cached
};

String BallMaker::Key() const
{
	char h[sizeof(int)+3];
	*(int *)h = r;
	h[sizeof(int)] = color.GetR();
	h[sizeof(int)+1] = color.GetG();
	h[sizeof(int)+2] = color.GetB();
	return String(h, sizeof(int)+3);
}

Image BallMaker::Make() const
{
	return CreateBall(r,color);
}

Image CreateBallCached(int r, Color color)
{
	BallMaker m;
	m.r = r;
	m.color = color;
	return MakeImage(m);
}

struct MyApp : public TopWindow {
	bool cached;
	
	virtual void Paint(Draw& w) {
		w.DrawRect(GetSize(), White);
		for (int y=0; y<300; y+=30)
			for (int i=10;i<500;i+=i/3) {
				Color c = Color((200*i)&255, (150*i)&255, (300*i)&255);
				w.DrawImage(i,y+i/5, (cached ? CreateBallCached : CreateBall) (i/2, c));
			}
	}
	
	virtual void LeftDown(Point, dword) {
		cached = true;
		Title("Now cached - try to resize the window to see the speed");
	}
	
	MyApp() {
		cached = false;
		Title("Not cached - try to resize the window to see the speed," "Click to activate the cache");
		Sizeable().Zoomable();
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}

#include <math.h>
#include <stdio.h>
typedef struct {
  double x, y;
} vector;

void head(int x, int y) {
  printf(
      "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"%d\" height=\"%d\">\n",
      x, y);
  printf("<rect x=\"0\" y=\"0\" width=\"%d\" height=\"%d\" fill=\"white\"/>\n",
         x, y);
}
void text(vector p, char* text){
  printf("<text x=\"%lf\" y=\"%lf\" font-family=\"sans-serif\" font-size=\"40\" fill=\"black\">%s</text>", p.x, p.y, text);
}
void line(vector p1, vector p2) {
  printf(
      "<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" stroke=\"black\"/>\n",
      p1.x, p1.y, p2.x, p2.y);
}
void dashline1(vector p1, vector p2) {
  printf("<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" stroke=\"black\" "
         "stroke-dasharray = \"8 4\"/>\n",
         p1.x, p1.y, p2.x, p2.y);
}
void dashline2(vector p1, vector p2) {
  printf("<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" stroke=\"black\" "
         "stroke-width = \"2\" stroke-dasharray = \"18 12 4 12\"/>\n",
         p1.x, p1.y, p2.x, p2.y);
}
void dashcircle(vector p, double r) {
  printf("<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" fill=\"none\" "
         "stroke=\"#000\" stroke-dasharray = \"8 4\"/>\n",
         p.x, p.y, r);
}
void arc(vector p1, vector p2, double r) {
  printf("<path d=\"M %lf,%lf A %lf %lf 0 0 1 %lf,%lf\" fill=\"none\" "
         "stroke=\"black\"/>",
         p1.x, p1.y, r, r, p2.x, p2.y);
}
double rad(double deg) { return (deg * M_PI / 180); }
vector rotate(vector p, vector center, double rad) {
  double c = cos(rad), s = sin(rad);
  double xc = p.x * c, ys = p.y * s, xs = p.x * s, yc = p.y * c;
  p.x = xc - ys;
  p.y = xs + yc;
  p.x += center.x;
  p.y += center.y;
  return p;
}
int in_view(vector p, int iw, int ih) {
  double w = iw, h = ih;
  return ((0.0 <= p.x && p.x <= w) && (0.0 <= p.y && p.y <= h));
}
void one_rack(vector ref, vector center, double rot, double m, double a) {
  double hk = m, hf = 1.25 * m, p = M_PI * m, s = p / 2,
         R = 0.38 * m;
  double sa = sin(a);
  double ca = cos(a);
  double ta = tan(a);
  vector p1 = {ref.x - s, ref.y};
  double x1 = R * (1 / ca - ca);
  double y1 = ref.y-hf +R-R*sa;
  double hf_R = hf - R;
  vector p2 = {p1.x + hf_R * ta + x1, y1};
  vector p3 = {p2.x + R * ca, ref.y-hf};
  vector p6 = ref;
  vector p5 = {p6.x - hf_R * ta - x1, y1};
  vector p4 = {p5.x - R * ca, ref.y-hf};
  vector p7 = {ref.x + hk * tan(a), ref.y + hk};
  vector p8 = {ref.x + s - hk * tan(a), ref.y + hk};
  vector p9 = {ref.x + s, ref.y};
  line(rotate(p1,center,rot),rotate(p2,center,rot));
  arc(rotate(p2,center,rot),rotate(p3,center,rot),R);
  line(rotate(p3,center,rot),rotate(p4,center,rot));
  arc(rotate(p4,center,rot),rotate(p5,center,rot),R);
  line(rotate(p5,center,rot),rotate(p7,center,rot));
  line(rotate(p7,center,rot),rotate(p8,center,rot));
  line(rotate(p8,center,rot),rotate(p9,center,rot));
}
void rack(vector ref, vector center, double rot, double m, double a) {
  double p = M_PI*m;
  for (int i = -4; i <= 4; i++) {
    one_rack((vector){ref.x-p*i, ref.y}, center, rot, m, a);
  }
}
int main(void) {
  int screen_witdh = 2970, screen_height = 2100;
  head(screen_witdh, screen_height);

  vector center = {(double)screen_witdh / 2, 100};
  dashline2((vector){0, center.y}, (vector){screen_witdh, center.y});
  dashline2((vector){center.x, 0}, (vector){center.x, screen_height});

  double m = 20;
  m *= 10;
  double a = 20;
  int z = 16;

  double d = m * z;

  dashcircle(center, d / 2);
  char diameter[10];
  sprintf(diameter, "Φ%.2lf",d/10);
  text(center, diameter);

  double angle = 4, maxangle = 52;

  for (int i = -maxangle; i <= maxangle; i += angle) {
    dashline1(center, rotate((vector){0.0, d / 2 * 1.23}, center, rad(i)));
    dashline1(
        rotate((vector){-d / 2 * tan(rad(angle * 8)), d / 2}, center, rad(i)),
        rotate((vector){d / 2 * tan(rad(angle * 8)), d / 2}, center, rad(i)));
  }
  for (int i = -maxangle; i <= maxangle; i += angle) {
    rack((vector){d/2*rad(i),d/2}, center, rad(i), m, rad(a));
  }

  printf("</svg>");
  return 0;
}

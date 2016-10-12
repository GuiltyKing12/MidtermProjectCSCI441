#include "track.h"

Track::Track() {
  load_points("terrain_pts.csv");
  calculate_lookup();
  parametric_t = 0;
    res = 100;
    step = 5;
}

Track::Track(std::string fn) {
  load_points(fn);
  calculate_lookup();
  parametric_t = 0;
    res = 100;
    step = segments;
}

void Track::draw() {
  glPushMatrix();
    glColor3f(1, 1, 0);
    glBegin(GL_LINES);
      for (int t = 0; t < segments * res; t += 1) {
        vertex(get_point((float)t / res));
        vertex(get_point((float)(t + 1) / res));
      }
    glEnd();
  glPopMatrix();
}

void Track::load_points(std::string fn) {
  std::ifstream infile(fn.c_str());
  std::string line;
  getline(infile, line);
  int patch_sl = atoi(line.c_str());
  for (int i = 0; i < patch_sl * patch_sl * 16; i += 1) {
    getline(infile, line);
  }

  getline(infile, line);
  segments = atoi(line.c_str());

  control_points = new Point*[segments];
  for (int i = 0; i < segments; i += 1) {
    control_points[i] = new Point[4];
  }

  for (int i = 0; i < segments * 4; i += 1) {
    if (!getline(infile, line)) {
      break;
    }

    float x = atof(line.substr(0, line.find_first_of(',')).c_str());
    float y = atof(line.substr(line.find_first_of(',') + 1, line.find_last_of(',') - line.find_first_of(',') - 1).c_str());
    float z = atof(line.substr(line.find_last_of(',') + 1).c_str());

    Point p(x, y, z);
    int inner_index = i % 4;
    int outer_index = i / 4;
    control_points[outer_index][inner_index] = p;
  }

  infile.close(); 
}

void Track::vertex(Point p) {
  glVertex3f(p.x, p.y, p.z);
}

Point Track::get_point(float t) {
  int o = 0;
  while (t > 1.0) {
    o += 1;
    t -= 1.0;
  }

  return bez_curve(t, 
    control_points[o][0], control_points[o][1],
    control_points[o][2], control_points[o][3]);
}

Point Track::bez_curve(float t, Point p0, Point p1, Point p2, Point p3) {
  float t2 = t * t;
  float t3 = t2 * t;
  float mt = 1.0 - t;
  float mt2 = mt * mt;
  float mt3 = mt2 * mt;

  return p0 * mt3 + p1 * (3.0 * mt2 * t) + p2 * (3.0 * mt * t2) + p3 * t3;
}

void Track::move() {
    parametric_t += step;
    if(parametric_t > (segments * res) - 1) parametric_t = (int)parametric_t % (segments * res - 1);
}

Point Track::parametric_pos() {
    return get_point(parametric_t / res);
}

Vector Track::parametric_dir() {
    Vector dir = get_point(parametric_t / res) - get_point((parametric_t + 1) / res);
    return dir / dir.magnitude();
}

Vector Track::parametric_normal() {
    Vector n = parametric_dir().Cross(Vector(1, 0, 0));
    if (n.y < 0) {
      n = n * -1;
    }
    return n;
}

Point Track::arc_pos() {
    int a = 0;
    int b = 1;
    for (int i = 0; i < segments * 100; i += 1) {
      if (lookup_table[i] > arc_t) {
        a = i - 1;
        b = i;
        break;
      }
    }

    float offset = (arc_t - lookup_table[a]) / (lookup_table[b] - lookup_table[a]);
    float t = (1 - offset) * ((float)a / 100) + offset * ((float)b / 100);

    arc_t += (max_dist / 100);
    if (arc_t >= max_dist) {
      arc_t -= max_dist;
    }
    return get_point(t);
}

Vector Track::arc_dir() {
    int a = 0;
    int b = 1;
    for (int i = 0; i < segments * 100; i += 1) {
      if (lookup_table[i] > arc_t) {
        a = i - 1;
        b = i;
        break;
      }
    }

    float offset = (arc_t - lookup_table[a]) / (lookup_table[b] - lookup_table[a]);
    float t = (1 - offset) * ((float)a / 100) + offset * ((float)b / 100);
    float nt = t + 0.05;
    if (nt >= segments) {
      nt -= segments;
    }

    return get_point(nt) - get_point(t);
}

Vector Track::arc_normal() {
  return arc_dir().Cross(Vector(1, 0, 0));
}

void Track::calculate_lookup() {
    lookup_table[0] = 0;
    for (int t = 1; t < segments * 100; t += 1) {
      float dist = (get_point((float)t / 100) - get_point((float)(t - 1) / 100)).magnitude();
      lookup_table[t] = lookup_table[t - 1] + dist;
    }
    max_dist = lookup_table[segments * 100 - 1];
}

/* HSTMATCH module=EBOOT.BIN start=0x0004d7dc end=0x0004e780 symbol=FUN_0004d7dc lang=c++ */
#pragma inline_depth(8)
struct V4;
extern "C" {
void FUN_00022434(V4 *, const V4 *, const V4 *);   /* sub */
void FUN_00022418(V4 *, const V4 *, const V4 *);   /* add */
void FUN_00022498(float, V4 *, const V4 *);        /* scale */
void FUN_000224f8(V4 *, const V4 *, const V4 *);   /* cross */
float FUN_000224d8(const V4 *, const V4 *);        /* dot */
void FUN_00022518(V4 *, const V4 *);               /* normalize3 */
void FUN_0002246c(float, V4 *, const V4 *, const V4 *); /* lerp */
float FUN_00022e44(float);                         /* sqrt */
float sqrtf(float);
}

struct V4 {
    float x, y, z, w;
    V4() {}
    V4(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
    V4(const V4 &o) : x(o.x), y(o.y), z(o.z), w(o.w) {}
    float Dot(const V4 &b) const { return FUN_000224d8(this, &b); }
    void Normalize() { FUN_00022518(this, this); w = 0.0f; }
    void SetNormal(const V4 &v) { FUN_00022518(this, &v); w = 0.0f; }
    void Lerp(const V4 &a, const V4 &b, float t) { FUN_0002246c(t, this, &a, &b); }
    void SetCross(const V4 &a, const V4 &b) { FUN_000224f8(this, &a, &b); }
    void operator-=(const V4 &b) { FUN_00022434(this, this, &b); }
    V4 operator-() const { return V4(-x, -y, -z, -w); }
} __attribute__((aligned(16)));

struct Sphere {
    V4 pos;     /* w = radius */
    V4 vel;
};

struct Hit {
    V4 pos;
    V4 point;
    V4 normal;
    float t;
    float m34;
};

static inline V4 Sub(const V4 &a, const V4 &b) { V4 r; FUN_00022434(&r, &a, &b); return r; }
static inline V4 Add(const V4 &a, const V4 &b) { V4 r; FUN_00022418(&r, &a, &b); return r; }
static inline V4 Scale(float s, const V4 &v) { V4 r; FUN_00022498(s, &r, &v); return r; }
static inline V4 Cross(const V4 &a, const V4 &b) { V4 r; FUN_000224f8(&r, &a, &b); return r; }
static inline V4 operator*(const V4 &v, float k) { return Scale(k, v); }
static inline V4 TriNormal(const V4 *t)
{
    V4 r;
    r.SetCross(Sub(t[1], t[0]), Sub(t[2], t[1]));
    return r;
}
static inline V4 At(const Sphere *s, float t) { return Add(s->pos, Scale(t, s->vel)); }
static inline V4 Move(const V4 &p, const V4 &v)
{
    V4 r = p;
    FUN_00022418(&r, &r, &v);
    r.w = 1.0f;
    return r;
}

static inline bool InTri(const V4 *v, int n, const V4 &m, const V4 &p)
{
    const V4 *prev = &v[n - 1];
    while (n--) {
        V4 c;
        c.SetCross(m, Sub(*v, *prev));
        if (c.Dot(Sub(*prev, p)) < -0.0005f)
            return false;
        prev = v;
        v++;
    }
    return true;
}

extern "C" int FUN_0004d7dc(const V4 *tri, Hit *hit, const Sphere *s)
{
    V4 n = TriNormal(tri);
    n.Normalize();
    float rad = s->pos.w;
    float rr = 1.005f * rad;
    V4 end = Move(s->pos, s->vel);
    float d1 = n.Dot(Sub(end, tri[0]));
    if (d1 > rr)
        return 0;
    V4 start(s->pos.x, s->pos.y, s->pos.z, 1.0f);
    float d0 = n.Dot(Sub(start, tri[0]));
    if (d0 < -rr)
        return 0;
    if (d1 - d0 > rad * 0.005f)
        return 0;
    if (rad < d1 && rad < d0)
        return 0;
    float nrad = -rad;
    if (nrad > d1 && nrad > d0)
        return 0;
    if (d0 >= 0.98f * rad) {
        float t;
        if (rad < d0) {
            t = (rad - d0) / (d1 - d0);
            if (t <= 0.0f)
                t = 0.0f;
            else if (t > 1.0f)
                t = 1.0f;
        } else {
            t = -1.0f;
        }
        if (hit->t > t) {
            V4 hp;
            if (t > 0.0f) {
                hp = At(s, t);
                hp.w = 1.0f;
            } else if (t < 0.0f) {
                hp = Add(start, n * (rad - d0));
            } else {
                hp = start;
            }
            if (InTri(tri, 3, -n, hp)) {
                if (t < 0.0f || rr > d0) {
                    hit->m34 = -(rr - d0) / rad;
                    t = 0.0f;
                }
                hit->t = t;
                hit->normal = n;
                hp.w = 1.0f;
                hit->pos = hp;
                hit->point = Sub(hp, Scale(rad, hit->normal));
                hit->point.w = 1.0f;
                return 1;
            }
        }
    }
    V4 vel = s->vel;
    int found = 0;
    const V4 *e[4] = { &tri[0], &tri[1], &tri[2], &tri[0] };
    float rad2 = rad * rad;
    float eps = 0.005f * rad;
    for (int i = 0; i < 3; i++) {
        V4 edge = Sub(*e[i + 1], *e[i]);
        V4 en;
        en.SetCross(vel, edge);
        en.Normalize();
        float dn = en.Dot(Sub(start, *e[i]));
        if (nrad <= dn && dn <= rad) {
            float h = sqrtf(rad2 - dn * dn);
            V4 bn;
            bn.SetCross(en, edge);
            bn.Normalize();
            float a0 = bn.Dot(Sub(end, *e[i]));
            if (a0 <= h) {
                float a1 = bn.Dot(Sub(start, *e[i]));
                if (a1 >= 0.98f * h) {
                    float u;
                    if (a0 == a1) {
                        u = 1.0f;
                    } else {
                        u = (h - a1) / (a0 - a1);
                        if (u > 1.0f)
                            u = 1.0f;
                    }
                    if (hit->t > u) {
                        V4 q;
                        V4 cn = Add(Scale(dn, en), Scale(h, bn));
                        cn.Normalize();
                        if (u <= 0.0f) {
                            q = Add(start, cn * eps);
                        } else {
                            q = At(s, u);
                            q.w = 1.0f;
                        }
                        if (edge.Dot(Sub(q, *e[i])) >= 0.0f) {
                            if (edge.Dot(Sub(q, *e[i + 1])) <= 0.0f) {
                                found = 1;
                                if (u <= 0.0f) {
                                    u = 0.0f;
                                    hit->m34 = -0.005f;
                                }
                                hit->t = u;
                                hit->normal = cn;
                                q.w = 1.0f;
                                hit->pos = q;
                                V4 ed;
                                ed.SetNormal(edge);
                                hit->point.Lerp(*e[i], *e[i + 1], ed.Dot(Sub(q, *e[i])) / ed.Dot(Sub(*e[i + 1], *e[i])));
                                hit->point.w = 1.0f;
                            }
                        }
                    }
                }
            }
        }
    }
    if (found)
        return 1;
    vel.Normalize();
    float lim = 0.98f * 0.98f * rad2;
    for (int j = 0; j < 3; j++) {
        const V4 &v = tri[j];
        float a = vel.Dot(Sub(end, v));
        if (a < nrad)
            continue;
        float b = vel.Dot(Sub(start, v));
        if (b > 0.0f)
            continue;
        V4 d;
        d = Sub(start, v);
        d.w = 0.0f;
        if (d.Dot(d) < lim)
            continue;
        float h;
        if (a < 0.0f)
            h = sqrtf(rad2 - a * a);
        else
            h = rad;
        V4 w = Sub(v, start);
        w -= vel * w.Dot(vel);
        w.w = 0.0f;
        float dist = FUN_00022e44(w.Dot(w));
        if (dist <= h) {
            float k = sqrtf(rad2 - dist * dist);
            float u;
            if (a == b) {
                u = 1.0f;
            } else {
                u = (-k - b) / (a - b);
                if (u > 1.0f)
                    u = 1.0f;
            }
            if (hit->t > u) {
                hit->point = v;
                hit->point.w = 1.0f;
                found = 1;
                hit->pos = At(s, u);
                hit->pos.w = 1.0f;
                hit->normal = Sub(hit->pos, hit->point);
                hit->normal.Normalize();
                hit->normal.w = 0.0f;
                if (u <= 0.0f) {
                    hit->pos = Add(start, hit->normal * eps);
                    hit->pos.w = 1.0f;
                    hit->m34 = -0.005f;
                    u = 0.0f;
                }
                hit->t = u;
            }
        }
    }
    return found;
}
// NEAR MISS: 999/1001 words, 648 differ. Every stack slot (all 80 vector temps, named locals and the e[] array)
// and the instruction selection match retail; what differs is register colouring:
// - FPR colouring: retail rad=f27, a=f26, h/b=f25, nrad=f24, rad2=f23, rr/dn/lim=f22, d1/t/eps=f21, d0/a0/u=f20;
//   ours rad=f23, lim=f27, h=f26, rr/dn=f25, d1/t/a0/u=f24, nrad=f22, d0/rad2=f21 (and 0.005f*rad kept in f20).
// - ours CSEs the face-part `rad*0.005f` with `eps` (kept in f20 across the whole function); retail recomputes eps
//   before the edge loop (lui/ori/mtc1 + mul.s f21). Writing the first one as 0.005f*s->pos.w breaks the CSE but
//   reloads s->pos.w (1004 words); an inline Tol(r) helper does not break it.
// - InTri loop registers (retail cur=s4, prev=s5, count=s0; ours permuted) and the edge loop e[] cursor (s5 vs s4).
// - 2-word gap: the 0x4e5e0 hit->pos x store order after At(), and the Scale() arg setup order (mov.s f12 into the
//   delay slot in retail) follow from the colouring.
// Slot layout lessons used: helpers At()/TriNormal()/operator*/member Lerp(a,b,t) and `V4 d; d = Sub(..)` each set
//   the temp's inline depth, which decides its slot pass (see docs/MWCC-NOTES.md 2026-11-10 0x4d7dc).
// Ruled out for colouring: hoisting all float locals to the top in retail register order or reversed (same 648).
// near miss from lane fe1 (unreviewed)

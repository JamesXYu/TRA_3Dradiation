#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <limits>
#include <numeric>
#include <optional>
#include <random>
#include <string>
#include <tuple>
#include <utility>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdlib>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Simple 3D vector struct with basic operations
struct Vec3 {
    double x;
    double y;
    double z;

    Vec3() : x(0.0), y(0.0), z(0.0) {}
    Vec3(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}

    Vec3 operator+(const Vec3& other) const { return {x + other.x, y + other.y, z + other.z}; }
    Vec3 operator-(const Vec3& other) const { return {x - other.x, y - other.y, z - other.z}; }
    Vec3 operator*(double s) const { return {x * s, y * s, z * s}; }
    Vec3 operator/(double s) const { return {x / s, y / s, z / s}; }

    Vec3& operator+=(const Vec3& other) { x += other.x; y += other.y; z += other.z; return *this; }
    Vec3& operator-=(const Vec3& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }
    Vec3& operator*=(double s) { x *= s; y *= s; z *= s; return *this; }
    Vec3& operator/=(double s) { x /= s; y /= s; z /= s; return *this; }
};

static inline double dot(const Vec3& a, const Vec3& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
static inline Vec3 cross(const Vec3& a, const Vec3& b) {
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}
static inline double length(const Vec3& v) { return std::sqrt(dot(v, v)); }
static inline Vec3 normalize(const Vec3& v) {
    double len = length(v);
    if (len <= 1e-12) return {0.0, 0.0, 0.0};
    return v / len;
}

struct Plane {
    Vec3 normal;
    Vec3 point; // any point on plane
};

struct PolygonWithTemp {
	std::vector<Vec3> vertices;
	double temperature;
};

struct ReceiverPoint {
	Vec3 origin;
	Vec3 normal;
};

// Compute plane from polygon vertices (assumes first 3 non-collinear define plane)
inline std::optional<Plane> getPolygonPlane(const std::vector<Vec3>& verts) {
    if (verts.size() < 3) return std::nullopt;
    Vec3 v1 = verts[1] - verts[0];
    Vec3 v2 = verts[2] - verts[0];
    Vec3 n = cross(v1, v2);
    double nmag = length(n);
    if (nmag < 1e-9) return std::nullopt;
    n = n / nmag;
    return Plane{n, verts[0]};
}

// Ray-plane intersection: returns intersection point and t, or nullopt/inf if no forward hit
inline std::pair<std::optional<Vec3>, double> rayPlaneIntersect(
    const Vec3& rayOrigin,
    const Vec3& rayDir,
    const Vec3& planeNormal,
    const Vec3& pointOnPlane
) {
    double ndotu = dot(planeNormal, rayDir);
    if (std::fabs(ndotu) < 1e-9) {
        return {std::nullopt, std::numeric_limits<double>::infinity()};
    }
    Vec3 w = rayOrigin - pointOnPlane;
    double t = -dot(planeNormal, w) / ndotu;
    if (t < 1e-7) {
        return {std::nullopt, std::numeric_limits<double>::infinity()};
    }
    Vec3 p = rayOrigin + rayDir * t;
    return {p, t};
}

// Project 3D polygon and point onto the dominant plane and do 2D point-in-polygon test (winding/non-zero)
static bool isPointInPolygon2D(const std::vector<std::array<double,2>>& poly, double x, double y) {
    // Ray casting even-odd rule
    bool inside = false;
    size_t n = poly.size();
    for (size_t i = 0, j = n - 1; i < n; j = i++) {
        const auto& pi = poly[i];
        const auto& pj = poly[j];
        bool intersect = ((pi[1] > y) != (pj[1] > y)) &&
                         (x < (pj[0] - pi[0]) * (y - pi[1]) / ((pj[1] - pi[1]) + 1e-30) + pi[0]);
        if (intersect) inside = !inside;
    }
    return inside;
}

inline bool isPointInPolygon3D(const Vec3& p, const std::vector<Vec3>& polygon, const Vec3& polygonNormal) {
    Vec3 absn { std::fabs(polygonNormal.x), std::fabs(polygonNormal.y), std::fabs(polygonNormal.z) };
    int a = 0, b = 1; // indices to keep
    if (absn.x >= absn.y && absn.x >= absn.z) { a = 1; b = 2; }
    else if (absn.y >= absn.x && absn.y >= absn.z) { a = 0; b = 2; }
    else { a = 0; b = 1; }

    std::vector<std::array<double,2>> poly2d;
    poly2d.reserve(polygon.size());
    for (const auto& v : polygon) {
        double coords[3] = {v.x, v.y, v.z};
        poly2d.push_back({coords[a], coords[b]});
    }
    double pc[3] = {p.x, p.y, p.z};
    return isPointInPolygon2D(poly2d, pc[a], pc[b]);
}

// Generate cosine-weighted hemisphere directions around a given normal (using provided RNG)
std::vector<Vec3> generateCosineHemisphereRays(size_t numRays, const Vec3& surfaceNormal, std::mt19937_64& rng) {
    std::vector<Vec3> rays;
    if (numRays == 0) return rays;
    rays.reserve(numRays);

    Vec3 w = normalize(surfaceNormal);
    Vec3 u;
    if (std::fabs(w.x) > 0.9999) {
        u = normalize(cross({0.0, 1.0, 0.0}, w));
    } else {
        u = normalize(cross({1.0, 0.0, 0.0}, w));
    }
	Vec3 v = cross(w, u);
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    for (size_t i = 0; i < numRays; ++i) {
        double u1 = dist(rng);
        double u2 = dist(rng);
        double phi = 2.0 * M_PI * u1;
        double cosTheta = std::sqrt(1.0 - u2);
        double sinTheta = std::sqrt(u2);
        double x = sinTheta * std::cos(phi);
        double y = sinTheta * std::sin(phi);
        double z = cosTheta;
        Vec3 local {x, y, z};
        // rotate to world
        Vec3 world {
            u.x * local.x + v.x * local.y + w.x * local.z,
            u.y * local.x + v.y * local.y + w.y * local.z,
            u.z * local.x + v.z * local.y + w.z * local.z
        };
        rays.push_back(world);
    }
    return rays;
}

// Convenience overload: non-deterministic RNG per call
std::vector<Vec3> generateCosineHemisphereRays(size_t numRays, const Vec3& surfaceNormal) {
	std::random_device rd;
	std::seed_seq seedSeq{rd(), rd(), rd(), rd(), rd(), rd()};
	std::mt19937_64 rng(seedSeq);
	return generateCosineHemisphereRays(numRays, surfaceNormal, rng);
}

// Calculate view factors from a point origin to a set of polygon emitters with occlusion between them
struct ViewFactorResult {
    std::vector<double> viewFactors; // per polygon
    std::vector<Vec3> allRayDirs;
    std::vector<Vec3> hitPoints;
    std::vector<Vec3> hitRayDirs; // those rays that hit some polygon
};

ViewFactorResult calculateViewFactorsWithBlockageLegacy(
    const Vec3& origin,
    const Vec3& originNormal,
    const std::vector<std::vector<Vec3>>& emitterPolygons,
    const std::vector<std::vector<Vec3>>& inertPolygons,
    size_t numRays
) {
    ViewFactorResult res;
    res.viewFactors.assign(emitterPolygons.size(), 0.0);
    if (numRays == 0) return res;

    std::vector<Vec3> rays = generateCosineHemisphereRays(numRays, originNormal);
    res.allRayDirs = rays;

    struct PolyData { std::vector<Vec3> verts; Vec3 normal; Vec3 point; bool valid; };
    std::vector<PolyData> inertScene;
    inertScene.reserve(inertPolygons.size());
    for (const auto& poly : inertPolygons) {
        auto pl = getPolygonPlane(poly);
        if (!pl) inertScene.push_back({poly, {0,0,0}, {0,0,0}, false});
        else inertScene.push_back({poly, pl->normal, pl->point, true});
    }

    std::vector<PolyData> emitScene;
    emitScene.reserve(emitterPolygons.size());
    for (const auto& poly : emitterPolygons) {
        auto pl = getPolygonPlane(poly);
        if (!pl) emitScene.push_back({poly, {0,0,0}, {0,0,0}, false});
        else emitScene.push_back({poly, pl->normal, pl->point, true});
    }

    std::vector<std::size_t> hitCounts(emitterPolygons.size(), 0);

    for (size_t i = 0; i < numRays; ++i) {
        const Vec3& rdir = rays[i];
        double closestInert = std::numeric_limits<double>::infinity();
        double closestEmit = std::numeric_limits<double>::infinity();
        int finalIdx = -1;
        Vec3 closestPoint;

        // Check inert polygons first (find closest inert hit)
        for (size_t p = 0; p < inertScene.size(); ++p) {
            const auto& pd = inertScene[p];
            if (!pd.valid) continue;
            auto [hit, t] = rayPlaneIntersect(origin, rdir, pd.normal, pd.point);
            if (hit) {
                if (t < closestInert) {
                    if (isPointInPolygon3D(*hit, pd.verts, pd.normal)) {
                        closestInert = t;
                    }
                }
            }
        }

        // Then check emitter polygons (find closest emitter hit)
        for (size_t p = 0; p < emitScene.size(); ++p) {
            const auto& pd = emitScene[p];
            if (!pd.valid) continue;
            auto [hit, t] = rayPlaneIntersect(origin, rdir, pd.normal, pd.point);
            if (hit) {
                if (t < closestEmit) {
                    if (isPointInPolygon3D(*hit, pd.verts, pd.normal)) {
                        closestEmit = t;
                        finalIdx = static_cast<int>(p);
                        closestPoint = *hit;
                    }
                }
            }
        }

        bool blockedByInert = (closestInert < std::numeric_limits<double>::infinity()) && (closestInert <= closestEmit);
        if (!blockedByInert && finalIdx != -1) {
            hitCounts[static_cast<size_t>(finalIdx)] += 1;
            res.hitPoints.push_back(closestPoint);
            res.hitRayDirs.push_back(rdir);
        }
    }

    for (size_t p = 0; p < emitterPolygons.size(); ++p) {
        res.viewFactors[p] = static_cast<double>(hitCounts[p]) / static_cast<double>(numRays);
    }
    return res;
}

// Overload that accepts external RNG for reproducible runs and JSON-driven seed

ViewFactorResult calculateViewFactorsWithBlockage(
	const Vec3& origin,
	const Vec3& originNormal,
	const std::vector<PolygonWithTemp>& emitterPolygons,
	const std::vector<std::vector<Vec3>>& inertPolygons,
	size_t numRays,
	std::mt19937_64& rng
) {
	ViewFactorResult res;
	res.viewFactors.assign(emitterPolygons.size(), 0.0);
	if (numRays == 0) return res;

	std::vector<Vec3> rays = generateCosineHemisphereRays(numRays, originNormal, rng);
	res.allRayDirs = rays;

	struct PolyData { std::vector<Vec3> verts; Vec3 normal; Vec3 point; bool valid; };
	std::vector<PolyData> inertScene;
	inertScene.reserve(inertPolygons.size());
	for (const auto& poly : inertPolygons) {
		auto pl = getPolygonPlane(poly);
		if (!pl) inertScene.push_back({poly, {0,0,0}, {0,0,0}, false});
		else inertScene.push_back({poly, pl->normal, pl->point, true});
	}

	std::vector<PolyData> emitScene;
	emitScene.reserve(emitterPolygons.size());
	for (const auto& poly : emitterPolygons) {
		auto pl = getPolygonPlane(poly.vertices);
		if (!pl) emitScene.push_back({poly.vertices, {0,0,0}, {0,0,0}, false});
		else emitScene.push_back({poly.vertices, pl->normal, pl->point, true});
	}

	std::vector<std::size_t> hitCounts(emitterPolygons.size(), 0);

	for (size_t i = 0; i < numRays; ++i) {
		const Vec3& rdir = rays[i];
		double closestInert = std::numeric_limits<double>::infinity();
		double closestEmit = std::numeric_limits<double>::infinity();
		int finalIdx = -1;
		Vec3 closestPoint;

		// Check inert polygons first (find closest inert hit)
		for (size_t p = 0; p < inertScene.size(); ++p) {
			const auto& pd = inertScene[p];
			if (!pd.valid) continue;
			auto [hit, t] = rayPlaneIntersect(origin, rdir, pd.normal, pd.point);
			if (hit) {
				if (t < closestInert) {
					if (isPointInPolygon3D(*hit, pd.verts, pd.normal)) {
						closestInert = t;
					}
				}
			}
		}

		// Then check emitter polygons (find closest emitter hit)
		for (size_t p = 0; p < emitScene.size(); ++p) {
			const auto& pd = emitScene[p];
			if (!pd.valid) continue;
			auto [hit, t] = rayPlaneIntersect(origin, rdir, pd.normal, pd.point);
			if (hit) {
				if (t < closestEmit) {
					if (isPointInPolygon3D(*hit, pd.verts, pd.normal)) {
						closestEmit = t;
						finalIdx = static_cast<int>(p);
						closestPoint = *hit;
					}
				}
			}
		}

		bool blockedByInert = (closestInert < std::numeric_limits<double>::infinity()) && (closestInert <= closestEmit);
		if (!blockedByInert && finalIdx != -1) {
			hitCounts[static_cast<size_t>(finalIdx)] += 1;
			res.hitPoints.push_back(closestPoint);
			res.hitRayDirs.push_back(rdir);
		}
	}

	for (size_t p = 0; p < emitterPolygons.size(); ++p) {
		res.viewFactors[p] = static_cast<double>(hitCounts[p]) / static_cast<double>(numRays);
	}
	return res;
}

// Minimal schema-specific JSON parser for our expected input
// Expected input JSON:
// {
//   "receiver_points": [
//     {
//       "origin": [x,y,z],
//       "normal": [nx,ny,nz]
//     },
//     ...
//   ],
//   "polygons": [
//     {
//       "polygon": [[x,y,z], ...],
//       "temperature": 100.0
//     },
//     ...
//   ],
//   "inert_polygons": [ [[x,y,z], ...], ... ],        // optional blockers only
//   "num_rays": 100000,          // optional (default 100000)
//   "seed": 123456789            // optional (deterministic if provided)
// }


namespace mini_json {
	inline void skipSpaces(const std::string& s, size_t& i) {
		while (i < s.size() && (s[i] == ' ' || s[i] == '\n' || s[i] == '\r' || s[i] == '\t')) ++i;
	}
	inline bool expectChar(const std::string& s, size_t& i, char c) {
		skipSpaces(s, i);
		if (i < s.size() && s[i] == c) { ++i; return true; }
		return false;
	}
	inline bool parseNumber(const std::string& s, size_t& i, double& out) {
		skipSpaces(s, i);
		const char* start = s.c_str() + i;
		char* endptr = nullptr;
		out = std::strtod(start, &endptr);
		if (endptr == start) return false;
		i += static_cast<size_t>(endptr - start);
		return true;
	}
	inline bool parseUInt64(const std::string& s, size_t& i, std::uint64_t& out) {
		skipSpaces(s, i);
		const char* start = s.c_str() + i;
		char* endptr = nullptr;
		unsigned long long v = std::strtoull(start, &endptr, 10);
		if (endptr == start) return false;
		out = static_cast<std::uint64_t>(v);
		i += static_cast<size_t>(endptr - start);
		return true;
	}
	inline bool parseKey(const std::string& s, size_t& i, const std::string& key) {
		skipSpaces(s, i);
		if (!expectChar(s, i, '"')) return false;
		size_t k = i;
		while (k < s.size() && s[k] != '"') ++k;
		if (k >= s.size()) return false;
		std::string found = s.substr(i, k - i);
		i = k + 1;
		skipSpaces(s, i);
		if (!expectChar(s, i, ':')) return false;
		return found == key;
	}
	inline bool parseVec3(const std::string& s, size_t& i, Vec3& v) {
		if (!expectChar(s, i, '[')) return false;
		double a, b, c;
		if (!parseNumber(s, i, a)) return false;
		if (!expectChar(s, i, ',')) return false;
		if (!parseNumber(s, i, b)) return false;
		if (!expectChar(s, i, ',')) return false;
		if (!parseNumber(s, i, c)) return false;
		if (!expectChar(s, i, ']')) return false;
		v = Vec3{a, b, c};
		return true;
	}
	inline bool parsePolygons(const std::string& s, size_t& i, std::vector<std::vector<Vec3>>& polys) {
		if (!expectChar(s, i, '[')) return false; // polygons array start
		skipSpaces(s, i);
		if (i < s.size() && s[i] == ']') { ++i; return true; }
		while (i < s.size()) {
			if (!expectChar(s, i, '[')) return false; // one polygon start
			std::vector<Vec3> poly;
			skipSpaces(s, i);
			if (i < s.size() && s[i] == ']') { ++i; polys.push_back(std::move(poly)); }
			else {
				while (i < s.size()) {
					Vec3 v;
					if (!parseVec3(s, i, v)) return false;
					poly.push_back(v);
					skipSpaces(s, i);
					if (i < s.size() && s[i] == ',') { ++i; continue; }
					if (i < s.size() && s[i] == ']') { ++i; break; }
					return false;
				}
				polys.push_back(std::move(poly));
			}
			skipSpaces(s, i);
			if (i < s.size() && s[i] == ',') { ++i; continue; }
			if (i < s.size() && s[i] == ']') { ++i; break; }
			return false;
		}
		return true;
	}
	inline bool parseReceiverPoint(const std::string& s, size_t& i, ReceiverPoint& rp) {
		if (!expectChar(s, i, '{')) return false;
		
		bool haveOrigin = false, haveNormal = false;
		
		while (i < s.size()) {
			skipSpaces(s, i);
			if (i < s.size() && s[i] == '}') { ++i; break; }
			
			size_t save = i;
			if (parseKey(s, i, "origin")) {
				Vec3 v; if (!parseVec3(s, i, v)) return false;
				rp.origin = v; haveOrigin = true;
			} else { i = save; }
			
			save = i;
			if (parseKey(s, i, "normal")) {
				Vec3 v; if (!parseVec3(s, i, v)) return false;
				rp.normal = v; haveNormal = true;
			} else { i = save; }
			
			skipSpaces(s, i);
			if (i < s.size() && s[i] == ',') { ++i; continue; }
		}
		
		return haveOrigin && haveNormal;
	}
	inline bool parseReceiverPoints(const std::string& s, size_t& i, std::vector<ReceiverPoint>& points) {
		if (!expectChar(s, i, '[')) return false;
		skipSpaces(s, i);
		if (i < s.size() && s[i] == ']') { ++i; return true; }
		
		while (i < s.size()) {
			ReceiverPoint rp;
			if (!parseReceiverPoint(s, i, rp)) return false;
			points.push_back(rp);
			
			skipSpaces(s, i);
			if (i < s.size() && s[i] == ',') { ++i; continue; }
			if (i < s.size() && s[i] == ']') { ++i; break; }
			return false;
		}
		return true;
	}
	
	inline bool parseReceiverPlaneData(const std::string& s, size_t& i, std::string& planeName, double& width, double& height, std::vector<ReceiverPoint>& points) {
		if (!expectChar(s, i, '{')) return false;
		skipSpaces(s, i);
		if (i < s.size() && s[i] == '}') { ++i; return true; }
		
		while (i < s.size()) {
			skipSpaces(s, i);
			if (i < s.size() && s[i] == '}') { ++i; break; }
			
			size_t save = i;
			if (parseKey(s, i, "width")) {
				double w;
				if (!parseNumber(s, i, w)) return false;
				width = w;
			} else { i = save; }
			
			save = i;
			if (parseKey(s, i, "height")) {
				double h;
				if (!parseNumber(s, i, h)) return false;
				height = h;
			} else { i = save; }
			
			save = i;
			if (parseKey(s, i, "points")) {
				if (!parseReceiverPoints(s, i, points)) return false;
			} else { i = save; }
			
			skipSpaces(s, i);
			if (i < s.size() && s[i] == ',') { ++i; continue; }
		}
		return true;
	}
	
	inline bool parseReceiverPlanes(const std::string& s, size_t& i, std::string& planeName, double& width, double& height, std::vector<ReceiverPoint>& points) {
		if (!expectChar(s, i, '{')) return false;
		skipSpaces(s, i);
		if (i < s.size() && s[i] == '}') { ++i; return true; }
		
		while (i < s.size()) {
			skipSpaces(s, i);
			if (i < s.size() && s[i] == '}') { ++i; break; }
			
			// Parse plane name as key
			if (!expectChar(s, i, '"')) return false;
			size_t nameStart = i;
			while (i < s.size() && s[i] != '"') ++i;
			if (i >= s.size()) return false;
			planeName = s.substr(nameStart, i - nameStart);
			++i; // skip closing quote
			
			skipSpaces(s, i);
			if (!expectChar(s, i, ':')) return false;
			
			if (!parseReceiverPlaneData(s, i, planeName, width, height, points)) return false;
			
			skipSpaces(s, i);
			if (i < s.size() && s[i] == ',') { ++i; continue; }
		}
		return true;
	}
	
	inline bool parseSinglePolygon(const std::string& s, size_t& i, std::vector<Vec3>& vertices) {
		if (!expectChar(s, i, '[')) return false;
		skipSpaces(s, i);
		if (i < s.size() && s[i] == ']') { ++i; return true; }
		
		while (i < s.size()) {
			Vec3 v;
			if (!parseVec3(s, i, v)) return false;
			vertices.push_back(v);
			
			skipSpaces(s, i);
			if (i < s.size() && s[i] == ',') { ++i; continue; }
			if (i < s.size() && s[i] == ']') { ++i; break; }
			return false;
		}
		return true;
	}
	
	inline bool parsePolygonWithTemp(const std::string& s, size_t& i, PolygonWithTemp& poly) {
		if (!expectChar(s, i, '{')) return false;
		
		bool havePolygon = false, haveTemperature = false;
		
		while (i < s.size()) {
			skipSpaces(s, i);
			if (i < s.size() && s[i] == '}') { ++i; break; }
			
			size_t save = i;
			if (parseKey(s, i, "polygon")) {
				if (!parseSinglePolygon(s, i, poly.vertices)) return false;
				havePolygon = true;
			} else { i = save; }
			
			save = i;
			if (parseKey(s, i, "temperature")) {
				double temp;
				if (!parseNumber(s, i, temp)) return false;
				poly.temperature = temp;
				haveTemperature = true;
			} else { i = save; }
			
			skipSpaces(s, i);
			if (i < s.size() && s[i] == ',') { ++i; continue; }
		}
		
		return havePolygon && haveTemperature;
	}
	
	inline bool parsePolygonsWithTemp(const std::string& s, size_t& i, std::vector<PolygonWithTemp>& polys) {
		if (!expectChar(s, i, '[')) return false;
		skipSpaces(s, i);
		if (i < s.size() && s[i] == ']') { ++i; return true; }
		
		while (i < s.size()) {
			PolygonWithTemp poly;
			if (!parsePolygonWithTemp(s, i, poly)) return false;
			polys.push_back(poly);
			
			skipSpaces(s, i);
			if (i < s.size() && s[i] == ',') { ++i; continue; }
			if (i < s.size() && s[i] == ']') { ++i; break; }
			return false;
		}
		return true;
	}
}

struct JsonInput {
	std::vector<ReceiverPoint> receiverPoints;  // All points from receiver_planes
	std::vector<PolygonWithTemp> polygons;           // emitter polygons with temperature
	std::vector<std::vector<Vec3>> inertPolygons;      // optional blockers
	std::size_t numRays {100000};
	std::optional<std::uint64_t> seed;
	
	// Plane information for output
	std::string planeName;
	double planeWidth;
	double planeHeight;
};

static bool parseInputJson(const std::string& json, JsonInput& out, std::string& error) {
	using namespace mini_json;
	size_t i = 0;
	skipSpaces(json, i);
	if (!expectChar(json, i, '{')) { error = "Expected '{'"; return false; }

	bool haveReceiverPlanes = false, havePolygons = false;

	while (i < json.size()) {
		skipSpaces(json, i);
		if (i < json.size() && json[i] == '}') { ++i; break; }

		// Try known keys
		size_t save = i;
		if (parseKey(json, i, "receiver_planes")) {
			if (!parseReceiverPlanes(json, i, out.planeName, out.planeWidth, out.planeHeight, out.receiverPoints)) { 
				error = "Invalid receiver_planes"; 
				return false; 
			}
			haveReceiverPlanes = true;
		} else { i = save; }

		save = i;
		if (parseKey(json, i, "polygons")) {
			// Try new format first (polygons with temperature)
			size_t savePos = i;
			if (parsePolygonsWithTemp(json, i, out.polygons)) {
				// Successfully parsed new format
				havePolygons = true;
			} else {
				// Try legacy format (simple array of polygons)
				i = savePos;
				std::vector<std::vector<Vec3>> legacyPolygons;
				if (parsePolygons(json, i, legacyPolygons)) {
					// Convert to new format with default temperature
					out.polygons.clear();
					for (const auto& poly : legacyPolygons) {
						out.polygons.push_back({poly, 0.0}); // Default temperature of 0
					}
					havePolygons = true;
				} else {
					error = "Invalid polygons format - neither new format (with temperature) nor legacy format (simple arrays)";
					return false;
				}
			}
		} else { i = save; }

		// optional inert polygons (blockers)
		save = i;
		if (parseKey(json, i, "inert_polygons")) {
			std::vector<std::vector<Vec3>> tmp;
			if (!parsePolygons(json, i, tmp)) { error = "Invalid inert_polygons"; return false; }
			out.inertPolygons = std::move(tmp);
		} else { i = save; }

		save = i;
		if (parseKey(json, i, "num_rays")) {
			double n; if (!parseNumber(json, i, n)) { error = "Invalid num_rays"; return false; }
			if (n < 0) n = 0; out.numRays = static_cast<std::size_t>(n);
		} else { i = save; }

		save = i;
		if (parseKey(json, i, "seed")) {
			std::uint64_t s; if (!parseUInt64(json, i, s)) { error = "Invalid seed"; return false; }
			out.seed = s;
		} else { i = save; }

		// optional comma
		skipSpaces(json, i);
		if (i < json.size() && json[i] == ',') { ++i; continue; }
	}

	// Validate required fields
	if (!haveReceiverPlanes) {
		error = "Must provide 'receiver_planes' field";
		return false;
	}
	
	if (out.receiverPoints.empty()) {
		error = "receiver_planes is empty";
		return false;
	}
	
	if (!havePolygons) { error = "Missing polygons"; return false; }
	return true;
}

static std::string runFromJsonString(const std::string& jsonInput, bool& ok) {
	JsonInput in;
	std::string err;
	if (!parseInputJson(jsonInput, in, err)) {
		ok = false;
		return std::string("{\"error\": \"") + err + "\"}\n";
	}

	std::mt19937_64 rng;
	if (in.seed.has_value()) {
		rng.seed(in.seed.value());
	} else {
		std::random_device rd;
		std::seed_seq seedSeq{rd(), rd(), rd(), rd(), rd(), rd()};
		rng = std::mt19937_64(seedSeq);
	}

	// Process receiver points and calculate temperature contributions
	size_t numPoints = in.receiverPoints.size();
	std::vector<double> pointTemperatures(numPoints, 0.0);
	
	for (size_t pointIdx = 0; pointIdx < in.receiverPoints.size(); ++pointIdx) {
		const auto& receiverPoint = in.receiverPoints[pointIdx];
		
		// Create a new RNG for each point to ensure deterministic results
		std::mt19937_64 pointRng = rng;
		if (in.seed.has_value()) {
			// Use different seeds for each point for better randomness
			pointRng.seed(in.seed.value() + pointIdx * 12345);
		}
		
		auto res = calculateViewFactorsWithBlockage(receiverPoint.origin, receiverPoint.normal, in.polygons, in.inertPolygons, in.numRays, pointRng);
		
		// Calculate temperature contribution from each polygon
		double totalTemperature = 0.0;
		for (size_t p = 0; p < in.polygons.size(); ++p) {
			double temperatureContribution = res.viewFactors[p] * in.polygons[p].temperature;
			totalTemperature += temperatureContribution;
		}
		
		pointTemperatures[pointIdx] = totalTemperature;
	}

	// Output in the requested format
	std::ostringstream out;
	out.setf(std::ios::fixed); 
	out << std::setprecision(6); // Reduced precision for cleaner output
	
	// Output plane information
	out << "Plane: " << in.planeName << "\n";
	out << "Width: " << in.planeWidth << "\n";
	out << "Height: " << in.planeHeight << "\n";
	
	// Output temperature values for all points
	for (size_t i = 0; i < pointTemperatures.size(); ++i) {
		if (i > 0) out << " ";
		out << pointTemperatures[i];
	}
	out << "\n";
	
	ok = true;
	return out.str();
}

static bool fileExists(const std::string& path) {
	std::ifstream f(path);
	return f.good();
}

static bool readFileText(const std::string& path, std::string& out, std::string& error) {
	if (!fileExists(path)) {
		error = "File does not exist: " + path;
		return false;
	}
	
	std::ifstream f(path, std::ios::in | std::ios::binary);
	if (!f) { 
		error = "Cannot open file: " + path; 
		return false; 
	}
	std::ostringstream ss; ss << f.rdbuf();
	out = ss.str();
	return true;
}

int main(int argc, char* argv[]) {
	// Check if file path is provided as command line argument
	if (argc < 2) {
		std::cerr << "{\"error\": \"Usage: " << argv[0] << " <json_file_path>\"}\n";
		return 64; // usage error
	}
	
	std::string jsonFilePath = argv[1];
	
	// Remove any quotes that might have been copied from file explorer
	if (!jsonFilePath.empty() && jsonFilePath.front() == '"' && jsonFilePath.back() == '"') {
		jsonFilePath = jsonFilePath.substr(1, jsonFilePath.length() - 2);
	}
	
	// Remove any leading/trailing whitespace
	jsonFilePath.erase(0, jsonFilePath.find_first_not_of(" \t\r\n"));
	jsonFilePath.erase(jsonFilePath.find_last_not_of(" \t\r\n") + 1);
	
	if (jsonFilePath.empty()) {
		std::cerr << "{\"error\": \"No file path provided\"}\n";
		return 64; // usage error
	}
	
	// Check if file exists
	if (!fileExists(jsonFilePath)) {
		std::cerr << "{\"error\": \"File does not exist: " << jsonFilePath << "\"}\n";
		return 1;
	}

	std::string jsonText, err;
	if (!readFileText(jsonFilePath, jsonText, err)) {
		std::cerr << "{\"error\": \"" << err << "\"}\n";
		return 1;
	}
	
	bool ok = false;
	std::string out = runFromJsonString(jsonText, ok);
	if (!ok) {
		std::cerr << out;
		return 2;
	}
	std::cout << out;
	return 0;
}



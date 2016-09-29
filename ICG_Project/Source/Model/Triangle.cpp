#include "Triangle.h"
#include "Transform.h"
#include "Model.h"

using namespace mdl;
using namespace math;
using namespace core;

Triangle::Triangle(const Vector3f &vA, const Vector3f &vB, const Vector3f &vC)
: m_vA(vA), m_vB(vB), m_vC(vC)
{
	vB.sub(vA, &m_eAB);
	vC.sub(vA, &m_eAC);

	m_eAB.cross(m_eAC, &m_nFace);
	m_nFace.normalizeLocal();

	m_nA.set(m_nFace);
	m_nB.set(m_nFace);
	m_nC.set(m_nFace);
}

Triangle::~Triangle()
{

}

boolean Triangle::isIntersect(const Ray &ray, Intersection *intersection) const
{
	Model *model = getModel();
	Vector3f localRayOrigin = model->getTransform().getInverseModelMatrix().mul(ray.getOrigin(), 1.0f);
	Vector3f localRayDir = getModel().getTransform().getInverseModelMatrix().mul(ray.getDir(), 0.0f).normalizeLocal();

	Ray localRay = new Ray(localRayOrigin, localRayDir);

	float dist = localRay.getOrigin().sub(m_vA).dot(m_normal) / (-localRay.getDir().dot(m_normal));

	// reject by distance
	if(dist < EPSILON || dist > Float.MAX_VALUE || dist != dist)
		return false;

	// projected hit point
	float hitPu, hitPv;

	// projected side vector AB and AC
	float abPu, abPv, acPu, acPv;

	// find dominant axis
	if(Math.abs(m_normal.x) > Math.abs(m_normal.y))
	{
		// X dominant, projection plane is YZ
		if(Math.abs(m_normal.x) > Math.abs(m_normal.z))
		{
			hitPu = dist * localRay.getDir().y + localRay.getOrigin().y - m_vA.y;
			hitPv = dist * localRay.getDir().z + localRay.getOrigin().z - m_vA.z;
			abPu = m_eAB.y;
			abPv = m_eAB.z;
			acPu = m_eAC.y;
			acPv = m_eAC.z;
		}
		// Z dominant, projection plane is XY
		else
		{
			hitPu = dist * localRay.getDir().x + localRay.getOrigin().x - m_vA.x;
			hitPv = dist * localRay.getDir().y + localRay.getOrigin().y - m_vA.y;
			abPu = m_eAB.x;
			abPv = m_eAB.y;
			acPu = m_eAC.x;
			acPv = m_eAC.y;
		}
	}
	// Y dominant, projection plane is ZX
	else if(Math.abs(m_normal.y) > Math.abs(m_normal.z))
	{
		hitPu = dist * localRay.getDir().z + localRay.getOrigin().z - m_vA.z;
		hitPv = dist * localRay.getDir().x + localRay.getOrigin().x - m_vA.x;
		abPu = m_eAB.z;
		abPv = m_eAB.x;
		acPu = m_eAC.z;
		acPv = m_eAC.x;
	}
	// Z dominant, projection plane is XY
	else
	{
		hitPu = dist * localRay.getDir().x + localRay.getOrigin().x - m_vA.x;
		hitPv = dist * localRay.getDir().y + localRay.getOrigin().y - m_vA.y;
		abPu = m_eAB.x;
		abPv = m_eAB.y;
		acPu = m_eAC.x;
		acPv = m_eAC.y;
	}

	// TODO: check if these operations are possible of producing NaNs

	// barycentric coordinate of vertex B in the projected plane
	float baryB = (hitPu*acPv - hitPv*acPu) / (abPu*acPv - abPv*acPu);
	if(baryB < 0.0f) return false;

	// barycentric coordinate of vertex C in the projected plane
	float baryC = (hitPu*abPv - hitPv*abPu) / (acPu*abPv - abPu*acPv);
	if(baryC < 0.0f) return false;

	if(baryB + baryC > 1.0f) return false;

	// so the ray intersects the triangle (TODO: reuse calculated results!)
	Vector3f localIntersectPoint = localRay.getDir().mul(dist).addLocal(localRay.getOrigin());
	Vector3f localIntersectNormal = new Vector3f(m_normal);
	//		Vector3f localIntersectNormal = m_nB.mul(baryB).addLocal(m_nC.mul(baryC)).addLocal(m_nA.mul(1.0f - baryB - baryC)).normalizeLocal();

	intersection.setHitAtomicPrimitive(this);
	intersection.setHitPoint(getModel().getTransform().getModelMatrix().mul(localIntersectPoint, 1.0f));
	intersection.setHitNormal(getModel().getTransform().getModelMatrix().mul(localIntersectNormal, 0.0f).normalizeLocal());

	return true;
}
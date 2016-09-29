#include "RawModel.h"

mdl::RawModel::RawModel(mdl::Primitive &primitive)
: m_primitive(&primitive), m_transform(new Transform())
{

}

mdl::RawModel::~RawModel()
{

}
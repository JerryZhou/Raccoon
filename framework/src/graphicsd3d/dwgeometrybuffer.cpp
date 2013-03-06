#include "stable.h"
#include "dwgeometrybuffer.h"
#include "dwgraphicsdatad3d_p.h"
#include "dwgraphicsd3d_p.h"
#include "dwtexture.h"
#include "dwgraphicseffect.h"

//----------------------------------------------------------------------------//
static const D3DMATRIX s_identityMatrix =
{
    1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 1.0
};

//------------------------------------------------------------------------------
/**
*/
DwGeometryBuffer::DwGeometryBuffer()
: DwGraphicsObject(new DwGeometryBufferData())
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwGeometryBuffer::~DwGeometryBuffer()
{
    clear();
}

//------------------------------------------------------------------------------
/**
*/
void DwGeometryBuffer::setTexture( const DwTexture *tex )
{
    const DwTextureData *dt = tex->data();
    DwGeometryBufferData *d = data();
    if (d->batchs.isEmpty() || d->batchs.back().textureData != dt)
    {
        d->batchs.append(DwBatch());
        d->batchs.back().textureData = (DwTextureData *)(dt);
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGeometryBuffer::addGeometry( const DwVertex* vbuff, int count )
{
    if(count<=0) return;
    DwGeometryBufferData *d = data();

    // update size of current batch
    d->batchs.back().len += count;

    // buffer these vertices
    DCustomVertex vd;
    const DwVertex* vs = vbuff;
    for (int i = 0; i < count; ++i, ++vs)
    {
        // copy vertex info the buffer, converting from Vertex to
        // something directly usable by D3D as needed.
        vd.position = D3DXVECTOR3(vs->x - 0.5f, vs->y - 0.5f, vs->z);
        vd.color = D3DCOLOR(vs->diffuse);
        vd.tu = vs->tu;
        vd.tv = vs->tv;
        d->vertexs.append(vd);
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGeometryBuffer::clear()
{
    DwGeometryBufferData *d = data();

    d->batchs.clear();
    d->vertexs.clear();
}

//------------------------------------------------------------------------------
/**
*/
void DwGeometryBuffer::draw()
{
    DwGeometryBufferData *d = data();

    if(!d->batchs.isEmpty())
    {
        updateMatrix();
        
        LPDIRECT3DDEVICE9 device = DwGraphicsD3D9::device();
        device->SetTransform(D3DTS_WORLD, &d->matrix);

        unsigned int pass = 0;
        DwGraphicsEffect effect(d->effectData);
        effect.begin( &pass , 0);
        for (unsigned int i=0; i<pass; ++i)
        {
            effect.beginPass(i);

            size_t pos = 0;
            DwBatchs::const_iterator ite = d->batchs.begin();
            while(ite != d->batchs.end())
            {
                HRESULT hr = device->SetTexture(0, (*ite).textureData->texture);
                hr = device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, (*ite).len / 3,
                    &d->vertexs[pos], sizeof(DCustomVertex));
                pos += (*ite).len;
                ++ite;
            }

            effect.endPass();
        }
        effect.end();
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGeometryBuffer::setTranslation(const DwVector3& t)
{
    DwGeometryBufferData *d = data();
    d->translation = t;
    d->matrixValid = false;
}

//------------------------------------------------------------------------------
/**
*/
void DwGeometryBuffer::setRotation(const DwVector3& r)
{
    DwGeometryBufferData *d = data();
    d->rotation = r;
    d->matrixValid = false;
}

//------------------------------------------------------------------------------
/**
*/
void DwGeometryBuffer::setPivot(const DwVector3& p)
{
    DwGeometryBufferData *d = data();
    d->pivot = p;
    d->matrixValid = false;
}

//------------------------------------------------------------------------------
/**
*/
void DwGeometryBuffer::setEffect(const DwGraphicsEffect *effect)
{
    DwGeometryBufferData *d = data();
    d->effectData = effect ? (DwGraphicsEffectData *)effect->data() : NULL;
}

//------------------------------------------------------------------------------
/**
*/
void DwGeometryBuffer::updateMatrix() const
{
    const DwGeometryBufferData *d = data();

    if (!d->matrixValid)
    {
        const D3DXVECTOR3 p(d->pivot.x, d->pivot.y, d->pivot.z);
        const D3DXVECTOR3 t(d->translation.x, d->translation.y, d->translation.z);

        D3DXQUATERNION r;
        D3DXQuaternionRotationYawPitchRoll(&r,
            D3DXToRadian(d->rotation.y),
            D3DXToRadian(d->rotation.x),
            D3DXToRadian(d->rotation.z));

        D3DXMatrixTransformation(&d->matrix, 0, 0, 0, &p, &r, &t);

        d->matrixValid = true;
    }
}
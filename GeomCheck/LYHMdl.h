/************************************************************************/
/*           Copyright (C) www.pisx.com, cad@pisx.com, 2017             */
/*                                                                      */
/*             FILE_NAME         :  LYHMdl.h                            */
/*             PRINCIPAL AUTHOR  :  Xuhaoran                            */
/*             SUBSYSTEM NAME    :  Independent                         */
/*             MODULE NAME       :  StringOperate                       */
/*             LANGUAGE          :  C++                                 */
/*             TARGET ENVIRONMENT:  Win7 x64 Creo 3.0                   */
/*             DATE OF CREATED   :  2017/12/13                          */
/*             DESCRIPTION       :  Visit & collect C style             */
/************************************************************************/

/*
 *Revision log:
 *
 *Created by Xuhaoran, 2017/12/13
 *
 */
#ifndef LYH_MDL_H
#define LYH_MDL_H

#include "stdafx.h"
#include "ProAsmcomppath.h"
#include <vector>
#include "UtilMath.h"
#include "BaseHole.h"
#include "PiXAABB.h"
#include "PiXHoleShiftCheck.h"

struct LYHAxis
{
	Pro3dPnt        m_orgin;
	ProVector       m_dir;
	BaseHole*       m_owner;
	ProAsmcomppath  m_path;

	LYHAxis();
	LYHAxis(Pro3dPnt origin, ProVector dir, BaseHole* owner, ProAsmcomppath& path);
	LYHAxis(const LYHAxis& r_axis);
	LYHAxis& operator=(const LYHAxis& r_axis);

	bool IsParallel(const ProVector& vec1, const ProVector& vec2);
	bool is_coaxial(const LYHAxis& r_axis);
};

class LYHMdl
{
public:
	ProAsmcomppath* m_p_path;

public:
	LYHMdl(): m_p_path(NULL){}
	LYHMdl(ProAsmcomppath* p_path): m_p_path(p_path){}

public:
	vector<BaseHole*> HoleCollect();
	BOOL HoleAxisCollect(vector<LYHAxis>& v_lyh_axis);
	//BOOL IsItself(ProAsmcomppath* p_path);
	bool operator==(const LYHMdl& r_lyh_mdl);
	BOOL MdlGet(ProMdl* mdl);
	//BOOL OutlineGet(Pro3dPnt outline[2]);
	//BOOL CouldBeAdjcent(LYHMdl& r_lyh_mdl);
	BOOL IsIntersects(LYHMdl& r_lyh_mdl);
	BOOL IsContact(LYHMdl& r_lyh_mdl);
	BOOL IsContact(LYHMdl& r_lyh_mdl, ProSurface& m_contact_surf, ProSurface& r_contact_surf);

	ProError ToSelection(ProSelection* p_sel);
};

ProError PlaneWithContrsSurfaceFilter(ProSurface p_surface, ProAppData);
bool     IsCoaxial(BaseHole* first_hole, ProAsmcomppath* first_path, BaseHole* second_hole, ProAsmcomppath* second_path);


#endif // !1






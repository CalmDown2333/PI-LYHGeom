/************************************************************************/
/*           Copyright (C) www.pisx.com, cad@pisx.com, 2017             */
/*                                                                      */
/*             FILE_NAME         :  LYHMdl.cpp                          */
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

#include "stdafx.h"
#include "LYHMdl.h"

LYHAxis::LYHAxis(): m_owner(NULL)
{
	ProUtilVectorCopy(NULL, m_orgin);
	ProUtilVectorCopy(NULL, m_dir);

	m_path.table_num = -1;
	m_path.owner = NULL;
	m_path.comp_id_table[0] = -1;
}

LYHAxis::LYHAxis(Pro3dPnt origin, ProVector dir, BaseHole* owner, ProAsmcomppath& path)
	: m_owner(owner), m_path(path)
{
	ProUtilVectorCopy(origin, m_orgin);
	ProUtilVectorCopy(dir, m_dir);

	m_path.table_num = path.table_num;
	m_path.owner = path.owner;
	for (int i = 0; i < m_path.table_num; ++i)
	{
		m_path.comp_id_table[i] = path.comp_id_table[i];
	}
}

LYHAxis::LYHAxis(const LYHAxis& r_axis): m_owner(r_axis.m_owner)
{
	ProUtilVectorCopy(const_cast<double*>(r_axis.m_orgin), m_orgin);
	ProUtilVectorCopy(const_cast<double*>(r_axis.m_dir), m_dir);

	m_path.table_num = r_axis.m_path.table_num;
	m_path.owner = r_axis.m_path.owner;
	for (int i = 0; i < r_axis.m_path.table_num; ++i)
	{
		m_path.comp_id_table[i] = r_axis.m_path.comp_id_table[i];
	}
}

LYHAxis& LYHAxis::operator=(const LYHAxis& r_axis)
{
	ProUtilVectorCopy(const_cast<double*>(r_axis.m_orgin), m_orgin);
	ProUtilVectorCopy(const_cast<double*>(r_axis.m_dir), m_dir);

	m_owner = r_axis.m_owner;
	m_path.table_num = r_axis.m_path.table_num;
	m_path.owner = r_axis.m_path.owner;
	for (int i = 0; i < r_axis.m_path.table_num; ++i)
	{
		m_path.comp_id_table[i] = r_axis.m_path.comp_id_table[i];
	}

	return *this;
}

bool LYHAxis::IsParallel(const ProVector& vec1, const ProVector& vec2)
{
	double tor = 0.000001;
	double x1 = fabs(vec2[0]) > tor ? vec1[0] / vec2[0] : 0;
	double x2 = fabs(vec2[1]) > tor ? vec1[1] / vec2[1] : 0;
	double x3 = fabs(vec2[2]) > tor ? vec1[2] / vec2[2] : 0;

	double scale = 0;
	if (fabs(x1) > tor)
		scale = x1;
	else if (fabs(x2) > tor)
		scale = x2;
	else if (fabs(x3) > tor)
		scale = x3;
	else
		return false;

	if (fabs(scale) < tor)
		return true;
	else
	{
		if (fabs(vec1[0] - vec2[0] * scale) < tor &&
			fabs(vec1[1] - vec2[1] * scale) < tor &&
			fabs(vec1[2] - vec2[2] * scale) < tor)
			return true;
		else
			return false;
	}

	return false;
}

bool LYHAxis::is_coaxial(const LYHAxis& r_axis)
{
	if (!IsParallel(m_dir, r_axis.m_dir)) return false;

	double offset = PiXHoleShiftDoctor::LineDistanceCompute(m_orgin, m_dir, const_cast<double*>(r_axis.m_orgin), const_cast<double*>(r_axis.m_dir));
	return (offset < 0.000001);

	//ProError status;
	//ProSrftype m_surf_type, r_surf_type;
	//vector<ProSurface> m_v_surf = m_owner->GetGeomSurfaces();
	//vector<ProSurface> r_v_surf = r_axis.m_owner->GetGeomSurfaces();

	//for (auto m_surf: m_v_surf)
	//{
	//	for (auto r_surf : r_v_surf)
	//	{
	//		status = ProSurfaceTypeGet(m_surf, &m_surf_type);
	//		status = ProSurfaceTypeGet(r_surf, &r_surf_type);

	//		if (m_surf_type == r_surf_type && m_surf_type == PRO_SRF_CYL)
	//		{
	//			status = SurfaceToSelection((ProSolid)m_owner->GetMdl(), m_surf, )
	//		}
	//	}
	//}

	//return false;
}

bool IsCoaxial(BaseHole* first_hole, ProAsmcomppath* first_path, BaseHole* second_hole, ProAsmcomppath* second_path)
{
	ProError     status;
	ProMdl       first_mdl;
	ProMdl       second_mdl;
	ProSelection first_sel;
	ProSelection second_sel;
	ProSrftype   first_surf_type, second_surf_type;
	vector<ProSurface> v_first_surf = first_hole->GetGeomSurfaces();
	vector<ProSurface> v_second_surf = second_hole->GetGeomSurfaces();

	status = ProAsmcomppathMdlGet(first_path, &first_mdl);
	status = ProAsmcomppathMdlGet(second_path, &second_mdl);

	for (auto first_surf: v_first_surf)
	{
		for (auto second_surf : v_second_surf)
		{
			status = ProSurfaceTypeGet(first_surf, &first_surf_type);
			status = ProSurfaceTypeGet(second_surf, &second_surf_type);

			if (first_surf_type == second_surf_type && first_surf_type == PRO_SRF_CYL)
			{
				status = SurfaceToSelection((ProSolid)first_mdl, first_surf, *first_path, &first_sel);
				status = SurfaceToSelection((ProSolid)second_mdl, second_surf, *second_path, &second_sel);
				double       distance;
				Pro2dPnt     pnt_2d_1, pnt_2d_2;
				Pro3dPnt     pnt_3d_1, pnt_3d_2;

				status = ProSelectionWithOptionsDistanceEval(first_sel, PRO_B_TRUE, second_sel, PRO_B_TRUE, pnt_2d_1, pnt_2d_2,
					pnt_3d_1, pnt_3d_2, &distance);

				return (fabs(distance) < 0.00001);
			}
		}
	}

	return false;
}

vector<BaseHole*> LYHMdl::HoleCollect()
{
	ProError status;
	ProMdl   mdl;

	if (!m_p_path) return vector<BaseHole*>();
	
	status = ProAsmcomppathMdlGet(m_p_path, &mdl);
	if (status) return vector<BaseHole*>();
	
	return PiBaseHoleCollect(mdl);
}

BOOL LYHMdl::HoleAxisCollect(vector<LYHAxis>& v_lyh_axis)
{
	vector<BaseHole*> v_hole = this->HoleCollect();

	v_lyh_axis.clear();

	for (auto item: v_hole)
	{
		Pro3dPnt  origin;
		ProVector dir;

		item->GetGeomOirgin(origin);
		item->GetGeomDirection(dir);

		PiPntTrfToAssembly(origin, *m_p_path);
		PiVecTrfToAssembly(dir, *m_p_path);

		v_lyh_axis.push_back(LYHAxis(origin, dir, item, *m_p_path));
	}


	return TRUE;
}

//BOOL LYHMdl::IsItself(ProAsmcomppath* p_path)
//{
//	if (m_p_path->owner != p_path->owner)
//	{
//		return FALSE;
//	}
//
//	if (m_p_path->table_num != p_path->table_num)
//	{
//		return FALSE;
//	}
//
//
//	for (int i = 0; i < m_p_path->table_num; ++i)
//	{
//		if (m_p_path->comp_id_table[i] != p_path->comp_id_table[i])
//		{
//			return FALSE;
//		}
//	}
//
//	return TRUE;
//}

bool LYHMdl::operator==(const LYHMdl& r_lyh_mdl)
{
	if (m_p_path->owner != r_lyh_mdl.m_p_path->owner)
	{
		return false;
	}

	if (m_p_path->table_num != r_lyh_mdl.m_p_path->table_num)
	{
		return false;
	}

	for (int i = 0; i < m_p_path->table_num; ++i)
	{
		if (m_p_path->comp_id_table[i] != r_lyh_mdl.m_p_path->comp_id_table[i])
		{
			return false;
		}
	}


	return true;
}

//BOOL LYHMdl::OutlineGet(Pro3dPnt outline[2])
//{
//	ProUtilVectorCopy(NULL, outline[0]);
//	ProUtilVectorCopy(NULL, outline[1]);
//
//	ProError status;
//	ProMdl   mdl;
//
//	if (!m_p_path) return FALSE;
//
//	status = ProAsmcomppathMdlGet(m_p_path, &mdl);
//	if (status) return FALSE;
//
//	status = ProSolidOutlineGet((ProSolid)mdl, outline);
//
//	PiPntTrfToAssembly(outline[0], *m_p_path);
//	PiPntTrfToAssembly(outline[1], *m_p_path);
//
//	return TRUE;
//}

BOOL LYHMdl::MdlGet(ProMdl* mdl)
{
	ProError status;
	if (!m_p_path) return FALSE;

	status = ProAsmcomppathMdlGet(m_p_path, mdl);
	if (status) return FALSE;

	return TRUE;
}

BOOL LYHMdl::IsIntersects(LYHMdl& r_lyh_mdl)
{
	ProError status;
	Pro3dPnt m_outline[2];
	Pro3dPnt r_outline[2];

	ProMdl   m_mdl;
	ProMdl   r_mdl;

	if (!this->MdlGet(&m_mdl)) return FALSE;
	
	if (!r_lyh_mdl.MdlGet(&r_mdl)) return FALSE;

	status = ProSolidOutlineGet((ProSolid)m_mdl, m_outline);
	if(status) return FALSE;
	status = ProSolidOutlineGet((ProSolid)r_mdl, r_outline);
	if(status) return FALSE;

	PiPntTrfToAssembly(m_outline[0], *m_p_path);
	PiPntTrfToAssembly(m_outline[1], *m_p_path);
	PiPntTrfToAssembly(r_outline[0], *m_p_path);
	PiPntTrfToAssembly(r_outline[1], *m_p_path);

	AABB m_aabb(m_outline[0], m_outline[1]);
	AABB r_aabb(r_outline[0], r_outline[1]);

	return (m_aabb.intersects(r_aabb));
}

ProError PlaneWithContrsSurfaceFilter(ProSurface p_surface, ProAppData)
{
	ProError status;
	ProSrftype tmp_srf_type;
	vector<ProContour> v_contr;

	status = ProSurfaceTypeGet(p_surface, &tmp_srf_type);
	if (tmp_srf_type != PRO_SRF_PLANE)//不为平面则跳过
	{
		return PRO_TK_CONTINUE;
	}

	status = PISX_xhr::PiSurfaceContoursCollect(p_surface, v_contr);
	if (v_contr.size() < 2)
	{
		return PRO_TK_CONTINUE;
	}


	return PRO_TK_NO_ERROR;
}

BOOL LYHMdl::IsContact(LYHMdl& r_lyh_mdl)
{
	//ProError status;
	//ProSelection first_sel, second_sel;
	//ProMdl first_mdl;
	//ProMdl second_mdl;
	//this->MdlGet(&first_mdl);
	//r_lyh_mdl.MdlGet(&second_mdl);
	//vector<ProSurface> v_first_planes;
	//vector<ProSurface> v_second_planes;

	//status = PISX_xhr::PiSolidSurfacesCollectWithFilter((ProSolid)first_mdl, v_first_planes, PlaneWithContrsSurfaceFilter);
	//status = PISX_xhr::PiSolidSurfacesCollectWithFilter((ProSolid)second_mdl, v_second_planes, PlaneWithContrsSurfaceFilter);

	//for (auto first_plane: v_first_planes)
	//{
	//	for (auto second_plane : v_second_planes)
	//	{
	//		status = SurfaceToSelection((ProSolid)first_mdl, first_plane, *this->m_p_path, &first_sel);
	//		status = SurfaceToSelection((ProSolid)second_mdl, second_plane, *r_lyh_mdl.m_p_path, &second_sel);
	//		double       distance;
	//		Pro2dPnt     pnt_2d_1, pnt_2d_2;
	//		Pro3dPnt     pnt_3d_1, pnt_3d_2;

	//		status = ProSelectionWithOptionsDistanceEval(first_sel, PRO_B_FALSE, second_sel, PRO_B_FALSE, pnt_2d_1, pnt_2d_2,
	//			pnt_3d_1, pnt_3d_2, &distance);
	//		if (fabs(distance) < 0.00001)
	//		{
	//			//AfxMessageBox(L"香蕉");
	//			return TRUE;
	//		}
	//	}
	//}	

	//return FALSE;

	ProSurface tmp_surf_1, tmp_surf_2;

	return IsContact(r_lyh_mdl, tmp_surf_1, tmp_surf_2);
}

BOOL LYHMdl::IsContact(LYHMdl& r_lyh_mdl, ProSurface& m_contact_surf, ProSurface& r_contact_surf)
{
	ProError status;
	ProSelection first_sel, second_sel;
	ProMdl first_mdl;
	ProMdl second_mdl;
	this->MdlGet(&first_mdl);
	r_lyh_mdl.MdlGet(&second_mdl);
	vector<ProSurface> v_first_planes;
	vector<ProSurface> v_second_planes;

	m_contact_surf = NULL;
	r_contact_surf = NULL;

	status = PISX_xhr::PiSolidSurfacesCollectWithFilter((ProSolid)first_mdl, v_first_planes, PlaneWithContrsSurfaceFilter);
	status = PISX_xhr::PiSolidSurfacesCollectWithFilter((ProSolid)second_mdl, v_second_planes, PlaneWithContrsSurfaceFilter);

	for (auto first_plane: v_first_planes)
	{
		for (auto second_plane : v_second_planes)
		{
			status = SurfaceToSelection((ProSolid)first_mdl, first_plane, *this->m_p_path, &first_sel);
			status = SurfaceToSelection((ProSolid)second_mdl, second_plane, *r_lyh_mdl.m_p_path, &second_sel);
			double       distance;
			Pro2dPnt     pnt_2d_1, pnt_2d_2;
			Pro3dPnt     pnt_3d_1, pnt_3d_2;

			status = ProSelectionWithOptionsDistanceEval(first_sel, PRO_B_FALSE, second_sel, PRO_B_FALSE, pnt_2d_1, pnt_2d_2,
				pnt_3d_1, pnt_3d_2, &distance);
			if (fabs(distance) < 0.00001)
			{
				//AfxMessageBox(L"香蕉");
				m_contact_surf = first_plane;
				r_contact_surf = second_plane;
				return TRUE;
			}
		}
	}	

	return FALSE;
}

ProError LYHMdl::ToSelection(ProSelection* p_sel)
{
	ProError status;
	ProMdl       mdl;
	ProModelitem model;
	p_sel = NULL;

	if(!m_p_path) return PRO_TK_BAD_INPUTS;

	status = ProAsmcomppathMdlGet(m_p_path, &mdl);
	if (status) return PRO_TK_GENERAL_ERROR;

	status = ProMdlToModelitem(mdl, &model);
	if(status) return PRO_TK_GENERAL_ERROR;

	status = ProSelectionAlloc(m_p_path, &model, p_sel);

	return status;
}
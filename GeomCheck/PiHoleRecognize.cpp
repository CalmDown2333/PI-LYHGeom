/************************************************************************/
/*           Copyright (C) www.pisx.com, cad@pisx.com, 2018             */
/*                                                                      */
/*             FILE_NAME         :  PiHoleRecognize.cpp                 */
/*             PRINCIPAL AUTHOR  :  Xuhaoran                            */
/*             SUBSYSTEM NAME    :  GeomCheck                           */
/*             MODULE NAME       :  Check                               */
/*             LANGUAGE          :  C++                                 */
/*             TARGET ENVIRONMENT:  Win7 x64 Creo 3.0                   */
/*             DATE OF CREATED   :  2018/03/19                          */
/*             DESCRIPTION       :  Compute the geom_info in the hole   */
/************************************************************************/

/*
 *Revision log:
 *
 *Created by Xuhaoran, 2018/03/10
 *
 */

#include "stdafx.h"
#include "PiHoleRecognize.h"


vector<BaseHole*> PiBaseHoleCollect(ProMdl p_mdl)
{
	vector<BaseHole*> v_hole = BaseHole::GetBaseHoles(p_mdl);

	return v_hole;
}


ProError HoleDepthCompute(IN BaseHole* hole, OUT double& depth)
{
	ProError status;
	vector<ProSurface> tmp_surfs;
	vector<ProContour> tmp_contrs;
	vector<ProEdge>    tmp_edge;

	depth = -1;

	BaseHole::HoleType tmp_hole_type;
	tmp_hole_type = hole->GetHoleType();
	if (tmp_hole_type == BaseHole::TypeThrough || tmp_hole_type == BaseHole::TypeScrew || tmp_hole_type == BaseHole::TypeBlind || tmp_hole_type == BaseHole::TypeConeThrough || tmp_hole_type == BaseHole::TypeConeBlind)
	{
		tmp_surfs = hole->GetGeomSurfaces();

		if (!tmp_surfs.empty())
		{
			for (int j = 0; j < tmp_surfs.size(); ++j)
			{
				ProSrftype tmp_srf_type;
				status = ProSurfaceTypeGet(tmp_surfs[j], &tmp_srf_type);
				if (tmp_srf_type == PRO_SRF_CYL)
				{
					status = PISX_xhr::PiSurfaceContoursCollect(tmp_surfs[j], tmp_contrs);
					if (!tmp_contrs.empty())
					{
						status = PISX_xhr::PiContourEdgesCollect(tmp_surfs[0], tmp_contrs[0], tmp_edge);
						if (!tmp_edge.empty())
						{
							for (auto item : tmp_edge)
							{
								ProEnttype tmp_type;

								status = ProEdgeTypeGet(item, &tmp_type);
								if (tmp_type == PRO_ENT_LINE)
								{
									status = ProEdgeLengthEval(item, &depth);
									break;
								}
							}
						}
					}
				}
				break;
			}	
		}
	}


	return status;
}

ProError HoleDiameterCompute(IN BaseHole* hole, OUT double& diameter)
{
	ProError status;
	vector<ProSurface> tmp_surfs;
	vector<ProContour> tmp_contrs;
	vector<ProEdge>    tmp_edge;

	diameter = -1;

	BaseHole::HoleType tmp_hole_type;
	tmp_hole_type = hole->GetHoleType();
	if (tmp_hole_type == BaseHole::TypeThrough || tmp_hole_type == BaseHole::TypeBlind || tmp_hole_type == BaseHole::TypeConeThrough || tmp_hole_type == BaseHole::TypeConeBlind)
	{
		tmp_surfs = hole->GetGeomSurfaces();

		if (!tmp_surfs.empty())
		{
			for (int j = 0; j < tmp_surfs.size(); ++j)
			{
				ProSrftype tmp_srf_type;
				status = ProSurfaceTypeGet(tmp_surfs[j], &tmp_srf_type);
				if (tmp_srf_type == PRO_SRF_CYL)
				{
					status = PISX_xhr::PiSurfaceContoursCollect(tmp_surfs[j], tmp_contrs);
					if (!tmp_contrs.empty())
					{
						status = PISX_xhr::PiContourEdgesCollect(tmp_surfs[0], tmp_contrs[0], tmp_edge);
						if (!tmp_edge.empty())
						{
							for (auto item : tmp_edge)
							{
								ProEnttype tmp_type;

								status = ProEdgeTypeGet(item, &tmp_type);
								if (tmp_type == PRO_ENT_ARC)
								{
									ProGeomitemdata* p_data;
									status = ProEdgeDataGet(item, &p_data);
									if(status) continue;
									diameter = p_data->data.p_curve_data->arc.radius * 2;
									status = ProGeomitemdataFree(&p_data);
									break;
								}
							}
						}
					}
				}
				break;
			}	
		}
	}


	return status;
}

ProError ScrewDiameterCompute(IN BaseHole* hole, OUT double& base_diameter, OUT double& diameter)
{
	ProError status;
	base_diameter = -1;
	diameter      = -1;

	BaseHole::HoleType tmp_hole_type;
	tmp_hole_type = hole->GetHoleType();

	if(tmp_hole_type == BaseHole::TypeScrew)
	{
		//ProFeature* p_feat = hole->GetHoleFeature();

		//ProFeature  tmp_feat;
		//ProGeomitem tmp_geom;

		std::vector<ProSurface> v_surf = hole->GetGeomSurfaces();
		if (v_surf.empty())
		{
			return PRO_TK_BAD_INPUTS;
		}

		ProFeature* p_feat;
		p_feat =hole->GetHoleFeature();
		//status = ProSurfaceToGeomitem((ProSolid)hole->GetMdl(), v_surf[0], &tmp_geom);
		//if(status) return status;

		//status = ProGeomitemFeatureGet(&tmp_geom, &tmp_feat);
		//if(status) return status;

		ProElement  root;
		wchar_t*    screw_size_name;
		CString     screw_size;
		CString     tmp_str;
		base_diameter = hole->GetHoleDiameter();
		status = ProFeatureElemtreeExtract(p_feat, NULL, PRO_FEAT_EXTRACT_NO_OPTS, &root);
		if(status) return status;

		status = ProElementHoleScrewSizeGet(root, hole->GetMdl(), &screw_size_name);
		if(status) return status;

		screw_size = CString(screw_size_name);

		if(!AfxExtractSubString(tmp_str, screw_size, 0, L'x')) return status;

		tmp_str = tmp_str.Right(tmp_str.GetLength() - 1);

		diameter = _ttof(tmp_str);

		status = ProWstringFree(screw_size_name);
		if(status) return status;

		status = ProFeatureElemtreeFree(p_feat, root);
		if(status) return status;
	}

	return status;
}


ProError HoleDiameterCompute(IN BaseHole* hole_1st, IN BaseHole* hole_2nd, OUT double& diameter)
{
	ProError status;
	double   tmp_dia_1st;
	double   tmp_dia_2nd;


	status = HoleDiameterCompute(hole_1st, tmp_dia_1st);
	status = HoleDiameterCompute(hole_2nd, tmp_dia_2nd);

	diameter = tmp_dia_1st > tmp_dia_2nd ? tmp_dia_1st : tmp_dia_2nd;

	return status;
}


ProError HoleDepthCompute(IN CombHole* hole, OUT double& up_depth, OUT double& down_depth, OUT double& total_depth)
{
	ProError status;
	BaseHole* up_hole;
	BaseHole* down_hole;

	vector<ProSurface> tmp_surfs;

	up_hole = hole->GetMaxHole();
	down_hole = hole->GetMinHole();

	status = HoleDepthCompute(up_hole, up_depth);
	if(status) return status;

	status = HoleDepthCompute(down_hole, down_depth);
	if(status) return status;

	total_depth = up_depth + down_depth;

	return status;
}

ProError HoleDiameterCompute(IN CombHole* hole, OUT double& up_diameter, OUT double& down_diameter)
{
	ProError status;
	BaseHole* up_hole;
	BaseHole* down_hole;

	up_hole = hole->GetMaxHole();
	down_hole = hole->GetMinHole();






	status = HoleDiameterCompute(up_hole, up_diameter);
	if(status) return status;

	status = HoleDiameterCompute(down_hole, down_diameter);
	if(status) return status;

	return status;
}
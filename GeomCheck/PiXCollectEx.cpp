/************************************************************************/
/*           Copyright (C) www.pisx.com, cad@pisx.com, 2017             */
/*                                                                      */
/*             FILE_NAME         :  PiXCollectEx.cpp                    */
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
#include "PiXCollectEx.h"
#include "PiXPublic.h"
#include "PiXCollect.h"

namespace PISX_xhr{

//
template<typename T>
ProError PiCollectVisitAction(T obj, ProError status, ProAppData app_data)
{
	std::vector<T>* p_v_objs = (std::vector<T>*)app_data;
	p_v_objs->push_back(obj);
	return status;
}

template<typename T>
ProError PiCollectVisitAction(T* p_obj, ProError status, ProAppData app_data)
{
	std::vector<T>* p_v_objs = (std::vector<T>*)app_data;
	p_v_objs->push_back(*p_obj);
	return status;
}

//筛选函数
ProError PiFeatureFilterAction(ProFeature* p_feature, ProAppData app_data)
{
	ProError      status;
	ProBoolean    visible = PRO_B_FALSE;
	status = ProFeatureVisibilityGet(p_feature, &visible);
	if (!visible)
	{
		return  PRO_TK_CONTINUE;
	}

	return status;
}

//Collect parameter in the pro_modelitem
ProError PiModelParamsCollect(ProModelitem* p_modelitem, std::vector<ProParameter>& v_p_parameters)
{
	ProError status;
	v_p_parameters.clear();
	status = ProParameterVisit(p_modelitem, NULL, PiCollectVisitAction<ProParameter>, (ProAppData)&v_p_parameters);
	return status;
}

//Collect asmcomponents in Asm,just one level~
ProError PiAsmcompCollect(ProAssembly p_asm, std::vector<ProAsmcomp>& v_asmcomps)
{
	ProError status;
	v_asmcomps.clear();

	status = ProSolidFeatVisit((ProSolid)p_asm, PiCollectVisitAction<ProAsmcomp>, PiAsmcompFilterAction, (ProAppData)&v_asmcomps);

	return status;
}

//收集单层Asmcomppathes
ProError PiOneLevelAsmcomppathCollect(ProAssembly p_asm, ProAsmcomppath& r_path, int level, std::vector<ProAsmcomppath>& v_pathes)
{
	ProError status;
	std::vector<ProAsmcomp> v_asmcomps;
	ProMdl     tmp_mdl;
	ProMdlType tmp_mdl_type;


	status = PiAsmcompCollect(p_asm, v_asmcomps);

	for (int i = 0; i < v_asmcomps.size(); ++i)
	{
		r_path.comp_id_table[level]     = v_asmcomps[i].id;
		r_path.comp_id_table[level + 1] = -1;
		r_path.table_num                = level + 1;

		status = ProAsmcompMdlGet(&(v_asmcomps[i]), &tmp_mdl);
		if(status) continue;

		v_pathes.push_back(r_path);

		status = ProMdlTypeGet(tmp_mdl, &tmp_mdl_type);
		if (tmp_mdl_type == PRO_MDL_ASSEMBLY)
		{
			status = PiOneLevelAsmcomppathCollect((ProAssembly)tmp_mdl, r_path, level + 1, v_pathes);
		}

	}

	return status;
}

//收集asmcomppath
ProError PiAsmcomppathCollect(ProAssembly p_asm, std::vector<ProAsmcomppath>& v_pathes)
{
	ProError       status;
	ProAsmcomppath tmp_path;

	v_pathes.clear();

	if (p_asm == (ProAssembly)NULL)
	{
		return PRO_TK_BAD_INPUTS;
	}

	tmp_path.owner = (ProSolid)p_asm;

	status = PiOneLevelAsmcomppathCollect(p_asm, tmp_path, 0, v_pathes);

	return status;
}


//收集所有的特征
ProError PiSolidFeatCollect(ProSolid p_solid, std::vector<ProFeature>& v_p_feats)
{
	ProError status;
	v_p_feats.clear();
	status = ProSolidFeatVisit(p_solid, PiCollectVisitAction<ProFeature>, PiFeatureFilterAction, (ProAppData)&v_p_feats);
	return status;
}

//template<class Container>
//ProError PiSolidFeatCollect(ProSolid p_solid, Container<ProFeature>& v_p_feats)
//{
//	ProError status;
//	v_p_feats.clear();
//	status = ProSolidFeatVisit(p_solid, PiCollectVisitAction<ProFeature, Container>, PiFeatureFilterAction, (ProAppData)&v_p_feats);
//	return status;
//}

//坐标轴遍历函数:收集实体内所有坐标轴
ProError PiSolidCsysCollect(ProSolid p_solid, std::vector<ProCsys>& v_csys)
{
	ProError status;
	v_csys.clear();
	status = ProSolidCsysVisit(p_solid, PiCollectVisitAction<ProCsys>, NULL, (ProAppData)&v_csys);
	return status;
}

//利用过滤器收集实体面
ProError PiSolidSurfacesCollectWithFilter(ProSolid p_solid, std::vector<ProSurface>& v_srfs, ProSurfaceFilterAction filter)
{
	ProError status;
	v_srfs.clear();
	status = ProSolidSurfaceVisit(p_solid, PiCollectVisitAction<ProSurface>, filter, (ProAppData)&v_srfs);
	return status;
}

//收集实体面
ProError PiSolidSurfacesCollect(ProSolid p_solid, std::vector<ProSurface>& v_srfs)
{
	return PiSolidSurfacesCollectWithFilter(p_solid, v_srfs, NULL);
}

//利用过滤器收集轮廓
ProError PiSurfaceContoursCollectWithFilter(ProSurface p_surface, std::vector<ProContour>& v_contours, ProSurfaceContourFilterAction filter)
{
	ProError status;
	v_contours.clear();
	status = ProSurfaceContourVisit(p_surface, PiCollectVisitAction<ProContour>, filter, (ProAppData)&v_contours);
	return status;
}

//收集轮廓
ProError PiSurfaceContoursCollect(ProSurface p_surface, std::vector<ProContour>& v_contours)
{
	return PiSurfaceContoursCollectWithFilter(p_surface, v_contours, NULL);
}

//收集边
ProError PiContourEdgesCollect(ProSurface p_srf, ProContour p_contour, std::vector<ProEdge>& v_edges)
{
	ProError status;
	v_edges.clear();
	status = ProContourEdgeVisit(p_srf, p_contour, PiCollectVisitAction<ProEdge>, NULL, (ProAppData)&v_edges);
	return status;
}

}
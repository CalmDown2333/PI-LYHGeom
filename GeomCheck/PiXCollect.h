/************************************************************************/
/*           Copyright (C) www.pisx.com, cad@pisx.com, 2017             */
/*                                                                      */
/*             FILE_NAME         :  PiXCollect.h                        */
/*             PRINCIPAL AUTHOR  :  Xuhaoran                            */
/*             SUBSYSTEM NAME    :  Independent                         */
/*             MODULE NAME       :  StringOperate                       */
/*             LANGUAGE          :  C                                   */
/*             TARGET ENVIRONMENT:  Win7 x64 Creo 3.0                   */
/*             DATE OF CREATED   :  2017/12/07                          */
/*             DESCRIPTION       :  Visit & collect C style             */
/************************************************************************/

/*
 *Revision log:
 *
 *Created by Xuhaoran, 2017/12/07
 *
 */

#ifndef PI_X_COLLECT_H
#define PI_X_COLLECT_H
#include "stdafx.h"

PI_BEGIN_XHR_NAMESPACE
ProError PiAsmcompFilterAction(ProFeature *feature, ProAppData app_data);
ProError PiCollect2ParamOpaqueVisitAction(void *, ProAppData);
ProError PiCollect3ParamOpaqueVisitAction(void *, ProError, ProAppData);
ProError PiCollect2ParamDBVisitAction(void *, ProAppData);
ProError PiCollect3ParamDBVisitAction(void *, ProError ,ProAppData);
ProError PiDefaultFilter();
ProError PiCollectSolidFeatures(ProSolid p_solid, ProFeature **p_features);
ProError PiCollectSolidFeaturesWithFilter(ProSolid p_solid, ProFeatureFilterAction filter, ProFeature **p_features);
ProError PiCollectAsmcomp(ProAssembly p_assy, ProAsmcomp** p_asmcomps);
ProError PiCollectOneLevelAsmcomppath(ProAssembly assy, ProAsmcomppath* p_path,int level,ProAsmcomppath **pp_path);
ProError PiCollectAsmcomppath(ProAssembly assy, ProAsmcomppath** pp_path, int* p_n_pathes);
ProError PiCollectMdlLayers(ProMdl model, ProLayer** p_layers);
ProError PiCollectRelset(ProMdl p_mdl, ProRelset** p_relset);
ProError PiCollectSolidSurfaces(ProSolid p_solid, ProSurface** p_surfaces);
ProError PiCollectSurfaceContours(ProSurface p_surface, ProContour** p_contours);
ProError PiCollectContourEdges(ProSurface p_surface, ProContour p_contour, ProEdge** p_edges);
ProError PiCollectDimVisitAction(ProDimension* dimension, ProError filter_staus, ProAppData data);
ProError PiCollectDimensions(ProMdl model, ProBoolean refdim, ProDimension** pp_dims);
PI_END_XHR_NAMESPACE
#endif
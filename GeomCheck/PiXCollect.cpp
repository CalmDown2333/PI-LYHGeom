/************************************************************************/
/*           Copyright (C) www.pisx.com, cad@pisx.com, 2017             */
/*                                                                      */
/*             FILE_NAME         :  PiXCollect.cpp                      */
/*             PRINCIPAL AUTHOR  :  Xuhaoran                            */
/*             SUBSYSTEM NAME    :  Independent                         */
/*             MODULE NAME       :  StringOperate                       */
/*             LANGUAGE          :  C                                   */
/*             TARGET ENVIRONMENT:  Win7 x64 Creo 3.0                   */
/*             DATE OF CREATED   :  2017/12/11                          */
/*             DESCRIPTION       :  Visit & collect C style             */
/************************************************************************/

/*
 *Revision log:
 *
 *Created by Xuhaoran, 2017/12/11
 *
 */

#include "stdafx.h"
#include "PiXCollect.h"

PI_BEGIN_XHR_NAMESPACE

//����PiAsmCompVisit()��������Ԫ������ʱ��ɸѡ����
ProError PiAsmcompFilterAction(ProFeature *feature,ProAppData app_data)
{
	ProError status;
	ProFeattype ftype;
	ProFeatStatus fstatus;

	//��ȡ��������
	status = ProFeatureTypeGet(feature, &ftype);
	status = ProFeatureStatusGet(feature, &fstatus);

	//���������Ԫ��������NO_ERROR�����򷵻�CONTINUE
	if(ftype == PRO_FEAT_COMPONENT && fstatus == PRO_FEAT_ACTIVE)
		return(PRO_TK_NO_ERROR);
	return(PRO_TK_CONTINUE);
}

//����ָ���õĶ�����ӵ������� ����PRO_TK_NO_ERROR��ʾִ�гɹ�
ProError PiCollect2ParamOpaqueVisitAction(
    void	   *p_object,	/* In:	The opaque handle being visited */
    ProAppData app_data)	/* In:	In fact it's ProArray** */
{
    return (PiCollect2ParamDBVisitAction((void*)&p_object, app_data));
}

//����ָ���õĶ�����ӵ������� ����PRO_TK_NO_ERROR��ʾִ�гɹ�
ProError PiCollect3ParamOpaqueVisitAction(
    void	   *p_object,	/* In:	The opaque handle being visited */
    ProError   status,		/* In:  The status returned by filter func */
    ProAppData app_data)	/* In:	In fact it's ProArray** */
{
    return (PiCollect2ParamDBVisitAction((void*)&p_object, app_data));
}

//����ָ���õĶ�����ӵ������� ����PRO_TK_NO_ERROR��ʾִ�гɹ�
ProError PiCollect2ParamDBVisitAction(
	void       *p_object,/* In:	The pointer to the object being visited */
    ProAppData app_data) /* In:	In fact it's ProArray** */
{
	ProError status; 
	ProArray *p_array;

	p_array = (ProArray*)((void**)app_data)[0];

	status = ProArrayObjectAdd(p_array, PRO_VALUE_UNUSED, 1, p_object );
	return (status);
}

//����ָ���õĶ�����ӵ������� ����PRO_TK_NO_ERROR��ʾִ�гɹ�
ProError PiCollect3ParamDBVisitAction(
    void	    *p_object,	/* In:	The pointer to the object being visited */
    ProError	    status,	/* In:  The status returned by filter func */
    ProAppData	  app_data)	/* In:	In fact it's ProArray** */
{
    return (PiCollect2ParamDBVisitAction(p_object, app_data));
}

//ɸѡ��������������ʹ�� ����PRO_TK_NO_ERROR ����������ж���
ProError PiDefaultFilter()
{

	return (PRO_TK_NO_ERROR);
}

//�ռ�ʵ������������ ����ֵ PRO_TK_NO_ERROR��ʾ����ִ����ȷ PRO_TK_BAD_INPUTS��ʾ��Ч����
ProError PiCollectSolidFeatures( 
    ProSolid	    p_solid,	    /* In:  The solid handle */
    ProFeature	    **p_features)   /* Out: ProArray with collected features
					                   items. The function allocates 
					                   memory for this argument, but 
					                   you must free it. To free 
					                   the memory, call the function 
					                   ProArrayFree() */

{
	ProError	    status;

	status =  PiCollectSolidFeaturesWithFilter (p_solid, 
		(ProFeatureFilterAction)PiDefaultFilter, p_features);
	return (status);
}

//�ռ�ʹ��ɸѡ������ʵ������������ ����ֵ PRO_TK_NO_ERROR��ʾ����ִ����ȷ PRO_TK_BAD_INPUTS��ʾ��Ч����
ProError PiCollectSolidFeaturesWithFilter( 
    ProSolid	    p_solid,	    /* In:  The solid handle */
    ProFeatureFilterAction filter,  /* In:  Filter function */
    ProFeature	    **p_features )  /* Out: ProArray with collected features
					                   items. The function allocates 
					                   memory for this argument, but 
					                   you must free it. To free 
					                   the memory, call the function 
					                   ProArrayFree() */

{
	ProError	    status;

	

	if( p_features != NULL )
	{
		status = ProArrayAlloc(0, sizeof(ProFeature), 1, (ProArray*)p_features);
		if( status == PRO_TK_NO_ERROR )
		{
				status = ProSolidFeatVisit( p_solid, 
				(ProFeatureVisitAction)PiCollect3ParamDBVisitAction,
				(ProFeatureFilterAction)filter,
				(ProAppData)&p_features );
			if( status != PRO_TK_NO_ERROR )
			{
				ProArrayFree( (ProArray*)p_features );
				*p_features = NULL;
			}
		}
	}
	else
		status = PRO_TK_BAD_INPUTS;

	return (status);
}

//�ռ�����е�����Ԫ�� ����ֵ PRO_TK_NO_ERROR��ʾ����ִ����ȷ PRO_TK_BAD_INPUTS��ʾ��Ч����
ProError PiCollectAsmcomp( 
    ProAssembly	    p_assy,	    /* In:  The assembly handle */
    ProAsmcomp **p_asmcomps)    /* Out: ProArray with collected asm 
					               comps. The function allocates 
					               memory for this argument, but 
					               you must free it. To free 
					               the memory, call the function 
				            	   ProArrayFree() */

{
	ProError status;
	status =  PiCollectSolidFeaturesWithFilter (p_assy, 
		(ProFeatureFilterAction)PiAsmcompFilterAction, p_asmcomps);

	return (status);
}

//�ռ�������Asmcomppathes�������������PiCollectAsmcomppath�б����� ע�� �벻Ҫֱ�ӵ����������
static ProError PiCollectOneLevelAsmcomppath(
	ProAssembly assy,
	ProAsmcomppath *p_path,
	int level,
	ProAsmcomppath **pp_path)
{
	ProError err;
	ProAsmcomp *p_comps;
	int i, n_comps;
	ProMdl mdl;
	ProMdlType mdl_type;

	err = PiCollectAsmcomp(assy, &p_comps);
	if (err != PRO_TK_NO_ERROR)
		return (err);

	err = ProArraySizeGet((ProArray)p_comps, &n_comps);
	for (i=0; i<n_comps; i++)
	{
		/*����Ԫ����asmcomppath�ṹ*/ 
		p_path->comp_id_table[level] = p_comps[i].id;
		p_path->comp_id_table[level + 1] = -1;
		p_path->table_num = level + 1;

		/*ȷ��Ԫ���ݹ���*/
		err = ProAsmcompMdlGet(p_comps+i, &mdl);
		if(err) continue;

		/* ��asmcomppath��ӵ�proarray*/
		ProArrayObjectAdd((ProArray*)pp_path, PRO_VALUE_UNUSED, 1, p_path);

		err = ProMdlTypeGet(mdl, &mdl_type);
		if (mdl_type == PRO_MDL_ASSEMBLY)
		{
			err = PiCollectOneLevelAsmcomppath((ProAssembly)mdl, p_path, 
				level+1, pp_path);
		}
	}
	err = ProArrayFree((ProArray*)&p_comps);
	return (PRO_TK_NO_ERROR);
}

//�ռ����������Ԫ�������������������װ��
ProError PiCollectAsmcomppath(
    ProAssembly assy,		    /* In : Assembly */
    ProAsmcomppath **pp_path,	/* Out: The ProArray of collected pathes.
					               The function allocates memory for 
					               this argument, but you must free it. 
					               To free the memory, call the function 
					               ProArrayFree() */
    int *p_n_pathes)		    /* Out: The number of pathes in the array */
{
    ProError err;
    ProAsmcomppath path;
    int n_pathes;
    
    if((assy == (ProAssembly)NULL) || pp_path == NULL )
	return (PRO_TK_BAD_INPUTS);

    err = ProArrayAlloc(0, sizeof(ProAsmcomppath), 10, (ProArray*)pp_path);
    path.owner = (ProSolid)assy;
    
    err = PiCollectOneLevelAsmcomppath(assy, &path, 0, pp_path);
    err = ProArraySizeGet((ProArray)*pp_path, &n_pathes);
    if (n_pathes<=0)
    {	
	err = ProArrayFree((ProArray*)pp_path);
	return (PRO_TK_E_NOT_FOUND);
    }

    p_n_pathes[0] = n_pathes;
    return (PRO_TK_NO_ERROR);
}

//�ռ�ʵ�������е�prolayer ����ֵ PRO_TK_NO_ERROR��ʾִ�гɹ� PRO_TK_BAD_INPUTS��ʾ��Ч����
ProError PiCollectMdlLayers( 
    ProMdl	    model,	      /* In:  The model handle */
    ProLayer **p_layers)      /* Out: ProArray with collected layers.
					             The function allocates 
					             memory for this argument, but 
					             you must free it. To free 
					             the memory, call the function 
					             ProArrayFree() */

{
    ProError	    status;

    status = ProMdlLayersCollect (model, p_layers);

    return (status);
}

//�ռ�ģ�������е�relset ����ֵ PRO_TK_NO_ERROR��ʾִ�гɹ� PRO_TK_BAD_INPUTS��ʾ��Ч����
ProError PiCollectRelset( 
    ProMdl	    p_mdl,	        /* In:  The model */
    ProRelset **p_relset)	    /* Out: ProArray with collected relsets. 
					               The function allocates 
					               memory for this argument, but 
					               you must free it. To free 
					               the memory, call the function 
					               ProArrayFree() */
{
    ProError	    status;

    if( p_relset != NULL )
    {
	status = ProArrayAlloc( 0, sizeof(ProRelset), 1, (ProArray*)p_relset );
	if( status == PRO_TK_NO_ERROR )
	{
	    status = ProSolidRelsetVisit( p_mdl, (ProSolidRelsetVisitAction)PiCollect2ParamDBVisitAction,(ProAppData)&p_relset );
	    if( status != PRO_TK_NO_ERROR )
	    {
		ProArrayFree( (ProArray*)p_relset );
		*p_relset = NULL;
	    }
	}
    }
    else
	status = PRO_TK_BAD_INPUTS;

    return (status);
}

//�ռ�ģ�������е�surface ����ֵ PRO_TK_NO_ERROR��ʾִ�гɹ� PRO_TK_BAD_INPUTS��ʾ��Ч����
ProError PiCollectSolidSurfaces( 
    ProSolid   p_solid,	    /* In:  The handle to the solid */
    ProSurface **p_surfaces /* Out: ProArray with collected surfaces. 
							The function allocates memory 
							for this argument, but you must 
							free it. To free the memory, 
							call the function ProArrayFree()*/
)
{
    ProError	    status;

    if( p_surfaces != NULL )
    {
	status = ProArrayAlloc(0,sizeof(ProSurface), 1, (ProArray*)p_surfaces);
	if( status == PRO_TK_NO_ERROR )
	{

	    status = ProSolidSurfaceVisit( p_solid, 
		(ProSurfaceVisitAction)PiCollect3ParamOpaqueVisitAction,
                (ProSurfaceFilterAction)PiDefaultFilter,
                (ProAppData)&p_surfaces );
	    if( status != PRO_TK_NO_ERROR )
	    {
		ProArrayFree( (ProArray*)p_surfaces );
		*p_surfaces = NULL;
	    }
	}
    }
    else
	status = PRO_TK_BAD_INPUTS;

    return (status);
}

//�ռ��������е����� ����ֵ PRO_TK_NO_ERROR��ʾִ�гɹ� PRO_TK_BAD_INPUTS��ʾ��Ч����
ProError PiCollectSurfaceContours( 
    ProSurface p_surfaces,	 /* In:  The handle to the quilt */
    ProContour **p_contours  /* Out: ProArray with collected contours. 
							 The function allocates memory 
							 for this argument, but you must 
							 free it. To free the memory, 
							 call the function ProArrayFree()*/
)
{
    ProError	    status;

    if( p_contours != NULL )
    {
	status = ProArrayAlloc( 0, sizeof(ProContour), 1, 
	    (ProArray*)p_contours );
	
	if( status == PRO_TK_NO_ERROR )
	{
	    status = ProSurfaceContourVisit( p_surfaces, 
		(ProSurfaceContourVisitAction)PiCollect3ParamOpaqueVisitAction,
                (ProSurfaceContourFilterAction)PiDefaultFilter,
                (ProAppData)&p_contours );
	    if( status != PRO_TK_NO_ERROR )
	    {
		ProArrayFree( (ProArray*)p_contours );
		*p_contours = NULL;
	    }
	}
    }
    else
	status = PRO_TK_BAD_INPUTS;

    return (status);
}

//�ռ����������еı� ����ֵ PRO_TK_NO_ERROR��ʾִ�гɹ� PRO_TK_BAD_INPUTS��ʾ��Ч����
ProError PiCollectContourEdges( 
    ProSurface p_surface,	    /* In:  The surface of the contour */
    ProContour p_contour,	    /* In:  The handle to the contour */
    ProEdge    **p_edges	    /* Out: ProArray with collected edges . 
								The function allocates memory 
							    for this argument, but you must 
							    free it. To free the memory, 
							    call the function ProArrayFree()*/
)
{
    ProError	    status;

    if( p_edges != NULL )
    {
	status = ProArrayAlloc( 0, sizeof(ProEdge), 1, (ProArray*)p_edges );
	if( status == PRO_TK_NO_ERROR )
	{
	    status = ProContourEdgeVisit( p_surface, p_contour, 
		(ProContourEdgeVisitAction)PiCollect3ParamOpaqueVisitAction,
                (ProContourEdgeFilterAction)PiDefaultFilter,
                (ProAppData)&p_edges );
	    if( status != PRO_TK_NO_ERROR )
	    {
		ProArrayFree( (ProArray*)p_edges );
		*p_edges = NULL;
	    }
	}
    }
    else
	status = PRO_TK_BAD_INPUTS;

    return (status);
}

//�ߴ������������
ProError PiCollectDimVisitAction (ProDimension *dimension,
								  ProError     filter_status, /* ignored */
								  ProAppData   data)
{
	ProDimension **p_dims = (ProDimension**)data;
	ProDimension cur_dim;

	cur_dim.owner = dimension->owner;
	cur_dim.id = dimension->id;
	cur_dim.type = dimension->type;
	ProArrayObjectAdd((ProArray*)p_dims, PRO_VALUE_UNUSED, 1, &cur_dim);

	return PRO_TK_NO_ERROR;
}


//�ռ����гߴ� ����ֵ PRO_TK_NO_ERROR��ʾִ�гɹ� PRO_TK_BAD_INPUTS��ʾ��Ч����
ProError PiCollectDimensions( 
    ProMdl	     model,	    /* In:  The handle to the model */
    ProBoolean   refdim,    /* In:  PRO_B_TRUE for collecting reference dimensions,
                                    PRO_B_FALSE for collecting standard dimensions */ 
    ProDimension **pp_dims   /* Out: ProArray with collected dimensions. 
							 The function allocates memory 
							 for this argument, but you must 
							 free it. To free the memory, 
							 call the function ProArrayFree()*/
)
{
    int n;
    ProError status;

    status = ProArrayAlloc( 0, sizeof(ProDimension), 1, (ProArray*)pp_dims);
    

    if( status == PRO_TK_NO_ERROR )
    {
      status = ProSolidDimensionVisit( (ProSolid)model, refdim,
                (ProDimensionVisitAction)PiCollectDimVisitAction,
                (ProDimensionFilterAction)NULL,
                (ProAppData)pp_dims);
    }




    ProArraySizeGet((ProArray*)pp_dims, &n);
    if (n==0)
    {
      status = PRO_TK_E_NOT_FOUND;
    }

    return (status);
}

PI_END_XHR_NAMESPACE
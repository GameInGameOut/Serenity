#pragma once

enum SendPropType
{
	DPT_Int = 0,
	DPT_Float,
	DPT_Vector,
	DPT_VectorXY,
	DPT_String,
	DPT_Array,
	DPT_DataTable,
	DPT_Int64,
	DPT_NUMSendPropTypes
};

class DVariant
{
public:
	union
	{
		float	   m_Float;
		long	   m_Int;
		char*    m_pString;
		void*    m_pData;
		float	   m_Vector[3];
		__int64  m_Int64;
	};
	SendPropType  m_Type;
};

class RecvTable;
class RecvProp;

class CRecvProxyData
{
public:
	const RecvProp*     m_pRecvProp;        // The property it's receiving.
	DVariant		    m_Value;            // The value given to you to store.
	int				    m_iElement;         // Which array element you're getting.
	int				    m_ObjectID;         // The object being referred to.
};

//-----------------------------------------------------------------------------
// pStruct = the base structure of the datatable this variable is in (like C_BaseEntity)
// pOut    = the variable that this this proxy represents (like C_BaseEntity::m_SomeValue).
//
// Convert the network-standard-type value in m_Value into your own format in pStruct/pOut.
//-----------------------------------------------------------------------------
typedef void(*RecvVarProxyFn)(const CRecvProxyData *pData, void *pStruct, void *pOut);

// ------------------------------------------------------------------------ //
// ArrayLengthRecvProxies are optionally used to Get the length of the 
// incoming array when it changes.
// ------------------------------------------------------------------------ //
typedef void(*ArrayLengthRecvProxyFn)(void *pStruct, int objectID, int currentArrayLength);

// NOTE: DataTable receive proxies work differently than the other proxies.
// pData points at the object + the recv table's offset.
// pOut should be Set to the location of the object to unpack the data table into.
// If the parent object just contains the child object, the default proxy just does *pOut = pData.
// If the parent object points at the child object, you need to dereference the pointer here.
// NOTE: don't ever return null from a DataTable receive proxy function. Bad things will happen.
typedef void(*DataTableRecvVarProxyFn)(const RecvProp *pProp, void **pOut, void *pData, int objectID);

class RecvProp
{
public:
	char*                   m_pVarName;
	SendPropType            m_RecvType;
	int                     m_Flags;
	int                     m_StringBufferSize;
	int                     m_bInsideArray;
	const void*             m_pExtraData;
	RecvProp*               m_pArrayProp;
	ArrayLengthRecvProxyFn  m_ArrayLengthProxy;
	RecvVarProxyFn          m_ProxyFn;
	DataTableRecvVarProxyFn m_DataTableProxyFn;
	RecvTable*              m_pDataTable;
	int                     m_Offset;
	int                     m_ElementStride;
	int                     m_nElements;
	const char*             m_pParentArrayPropName;

	void					SetProxyFn(RecvVarProxyFn fn);
	DataTableRecvVarProxyFn	GetDataTableProxyFn() const;
	void					SetDataTableProxyFn(DataTableRecvVarProxyFn fn);
	RecvTable* 				GetDataTable() const;
	int						GetOffset() const;
	RecvVarProxyFn		    GetProxyFn() const;

};

class RecvTable
{
public:
	RecvProp*               m_pProps;
	int                     m_nProps;
	void*                   m_pDecoder;
	char*                   m_pNetTableName;
	bool                    m_bInitialized;
	bool                    m_bInMainList;
};

__forceinline RecvTable* RecvProp::GetDataTable() const
{
	return m_pDataTable;
}
__forceinline int RecvProp::GetOffset() const
{
	return m_Offset;
}
__forceinline RecvVarProxyFn RecvProp::GetProxyFn() const
{
	return m_ProxyFn;
}

__forceinline void RecvProp::SetProxyFn(RecvVarProxyFn fn)
{
	m_ProxyFn = fn;
}

__forceinline DataTableRecvVarProxyFn RecvProp::GetDataTableProxyFn() const
{
	return m_DataTableProxyFn;
}


__forceinline void RecvProp::SetDataTableProxyFn(DataTableRecvVarProxyFn fn)
{
	m_DataTableProxyFn = fn;
}
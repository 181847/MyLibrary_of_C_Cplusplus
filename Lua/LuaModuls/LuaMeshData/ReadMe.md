模块
域   |   作用  | 类型
----|-----|----
new  |  创建一个新的MeshData |  function

meshData类成员方法
名称  | 作用    
---|----
show  |  向控制台基本的统计信息
getDetail  |  返回一个table，以相应的类型为键值，存储统计信息，包括*VertexCount,IndexCount,PositionCount,NormalCount,TangentUCount,TextureCoordCount**
addPosition  |  传入三个浮点数，作为一个顶点的坐标存储
addNormal  |  传入三个浮点数，作为一个法线存储
addTangentU  |  传入三个浮点数，作为切线向量存储
addTextureCoord  |  传入两个浮点数，作为UV坐标（贴图坐标）存储
addVertex  |  传入4个整形数，分别对应之前添加过的顶点坐标索引、贴图坐标索引、法线向量索引、切线向量索引，索引的最小值为1,0代表没有对应的信息。
addIndex  |  传入一个整形数字，对应addVertex中添加的顶点，最小值为1,0代表非法索引。
help  |  在屏幕上显示帮助信息

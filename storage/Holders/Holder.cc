

#include "storage/Holders/HolderImpl.h"
#include "storage/Devices/DeviceImpl.h"
#include "storage/DeviceGraph.h"
#include "storage/Utils/XmlFile.h"


namespace storage
{

    Holder::Holder(Impl* impl)
	: impl(impl)
    {
	if (!impl)
	    throw runtime_error("impl is nullptr");
    }


    Holder::~Holder()
    {
    }


    void
    Holder::create(DeviceGraph* device_graph, const Device* source, const Device* target)
    {
	addToDeviceGraph(device_graph, source, target);
    }


    void
    Holder::load(DeviceGraph* device_graph, const xmlNode* node)
    {
	sid_t source_sid = 0;
	if (!getChildValue(node, "source-sid", source_sid))
	    throw runtime_error("no source-sid");

	sid_t target_sid = 0;
	if (!getChildValue(node, "target-sid", target_sid))
	    throw runtime_error("no target-sid");

	const Device* source = device_graph->findDevice(source_sid);
	const Device* target = device_graph->findDevice(target_sid);

	addToDeviceGraph(device_graph, source, target);
    }


    Holder::Impl&
    Holder::getImpl()
    {
	return *impl;
    }


    const Holder::Impl&
    Holder::getImpl() const
    {
	return *impl;
    }


    sid_t
    Holder::getSourceSid() const
    {
	return getImpl().getSourceSid();
    }


    sid_t
    Holder::getTargetSid() const
    {
	return getImpl().getTargetSid();
    }


    void
    Holder::addToDeviceGraph(DeviceGraph* device_graph, const Device* source,
			     const Device* target)
    {
	if (source->getImpl().getDeviceGraph() != device_graph)
	    throw runtime_error("wrong graph in source");

	if (target->getImpl().getDeviceGraph() != device_graph)
	    throw runtime_error("wrong graph in target");

	DeviceGraph::Impl::vertex_descriptor source_vertex = source->getImpl().getVertex();
	DeviceGraph::Impl::vertex_descriptor target_vertex = target->getImpl().getVertex();

	pair<DeviceGraph::Impl::edge_descriptor, bool> tmp =
	    boost::add_edge(source_vertex, target_vertex, shared_ptr<Holder>(this),
			    device_graph->getImpl().graph);

	if (!tmp.second)
	    throw runtime_error("holder already exists");

	getImpl().setDeviceGraphAndEdge(device_graph, tmp.first);
    }


    void
    Holder::save(xmlNode* node) const
    {
	getImpl().save(node);
    }

}
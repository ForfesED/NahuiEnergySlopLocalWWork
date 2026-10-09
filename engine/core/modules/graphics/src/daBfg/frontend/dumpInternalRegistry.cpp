// Copyright 2024 N-GINN LLC. All rights reserved.

// Copyright (C) 2024  Gaijin Games KFT.  All rights reserved

#include "dumpInternalRegistry.h"
#include "nau/diag/logging.h"


void dabfg::dump_internal_registry(const InternalRegistry &registry)
{
  dump_internal_registry(
    registry, [](auto) { return true; }, [](auto) { return true; });
}

void dabfg::dump_internal_registry(const InternalRegistry &registry, NodeValidCb nodeValid, ResValidCb resourceValid)
{
  NAU_LOG_WARNING("Framegraph full user graph state dump:");
  for (auto [nodeId, nodeData] : registry.nodes.enumerate())
  {
    auto logNode = [&](NodeNameId id) { NAU_LOG_WARNING("\t\t'{}'{}", registry.knownNames.getName(id), !nodeValid(id) ? " (BROKEN)" : ""); };

    auto logRes = [&, &nodeData = nodeData](ResNameId id) {
      const auto &req = nodeData.resourceRequests.find(id)->second;
      NAU_LOG_WARNING("\t\t{}'{}'{}", req.optional ? "optional " : "", registry.knownNames.getName(id), !resourceValid(id) ? " (BROKEN)" : "");
    };

    auto dumpHelper = [](const auto &data, const char *heading, const auto &f) {
      if (data.empty())
        return;
      NAU_LOG_WARNING("\t{}", heading);
      for (const auto &d : data)
        f(d);
    };

    const bool broken = !nodeValid(nodeId);
    NAU_LOG_WARNING("Node '{}' ({}priority {})", registry.knownNames.getName(nodeId), broken ? "BROKEN, " : "", nodeData.priority);

    dumpHelper(nodeData.followingNodeIds, "Following nodes:", logNode);

    dumpHelper(nodeData.precedingNodeIds, "Previous nodes:", logNode);

    // TODO: implement a resource description pretty printer in d3d
    // and use it here

    dumpHelper(nodeData.createdResources, "Created resources:", logRes);

    dumpHelper(nodeData.readResources, "Read resources:", logRes);

    dumpHelper(nodeData.historyResourceReadRequests, "History read resources:", [&](const auto &pair) {
      NAU_LOG_WARNING("\t\t{}'{}'{}", pair.second.optional ? "optional " : "", registry.knownNames.getName(pair.first),
        !resourceValid(pair.first) ? " (BROKEN)" : "");
    });

    dumpHelper(nodeData.modifiedResources, "Modified resources:", logRes);

    dumpHelper(nodeData.renamedResources, "Renamed resources:", [&](auto pair) {
      const bool firstBroken = !resourceValid(pair.first);
      const bool secondBroken = !resourceValid(pair.second);
      // Yes, second is the old resource, first is the new one, this is not a typo.
      NAU_LOG_WARNING("\t\t'{}'{} -> '{}'{}", registry.knownNames.getName(pair.second), secondBroken ? " (BROKEN)" : "",
        registry.knownNames.getName(pair.first), firstBroken ? " (BROKEN)" : "");
    });
  }
  NAU_LOG_WARNING("Finished dumping framegraph state.");
}

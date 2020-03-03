// Copyright 2017-2020 The Verible Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// This unit provides helper functions that pertain to SystemVerilog
// module declaration nodes in the parser-generated concrete syntax tree.

#include "verilog/CST/macro.h"

#include "common/text/tree_utils.h"
#include "verilog/CST/verilog_matchers.h"
#include "verilog/CST/verilog_nonterminals.h"

namespace verilog {

using verible::Symbol;
using verible::SyntaxTreeNode;
using verible::TokenInfo;

std::vector<verible::TreeSearchMatch> FindAllMacroCalls(const Symbol& root) {
  return SearchSyntaxTree(root, NodekMacroCall());
}

const TokenInfo& GetMacroCallId(const Symbol& s) {
  return GetSubtreeAsLeaf(s, NodeEnum::kMacroCall, 0).get();
}

const SyntaxTreeNode& GetMacroCallArgs(const Symbol& s) {
  const auto& paren_group_node =
      GetSubtreeAsNode(s, NodeEnum::kMacroCall, 1, NodeEnum::kParenGroup);
  // See structure of (CST) MakeParenGroup().
  return GetSubtreeAsNode(paren_group_node, NodeEnum::kParenGroup, 1,
                          NodeEnum::kMacroArgList);
}

bool MacroCallArgsIsEmpty(const SyntaxTreeNode& args) {
  const auto& sub = CheckNodeEnum(args, NodeEnum::kMacroArgList).children();
  // Empty macro args are always constructed with one nullptr child in
  // the semantic actions in verilog.y.
  if (sub.size() != 1) return false;
  return sub.front() == nullptr;
}

}  // namespace verilog

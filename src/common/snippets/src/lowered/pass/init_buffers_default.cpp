// Copyright (C) 2023 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#include "snippets/lowered/pass/init_buffers_default.hpp"

#include <cstddef>

#include "snippets/itt.hpp"
#include "snippets/lowered/linear_ir.hpp"
#include "snippets/utils/utils.hpp"

namespace ov::snippets::lowered::pass {

bool InitBuffersDefault::run(lowered::LinearIR& linear_ir,
                             [[maybe_unused]] lowered::LinearIR::constExprIt begin,
                             [[maybe_unused]] lowered::LinearIR::constExprIt end) {
    OV_ITT_SCOPED_TASK(ov::pass::itt::domains::SnippetsTransform, "Snippets::InitBuffersDefault");

    size_t idx = 0;
    size_t offset = 0;
    for (const auto& buffer_expr : linear_ir.get_buffers()) {
        buffer_expr->set_reg_group(idx);
        buffer_expr->set_cluster_id(idx);

        if (!buffer_expr->is_defined()) {
            buffer_expr->set_offset(utils::get_dynamic_value<size_t>());
        } else {
            buffer_expr->set_offset(offset);
            offset += buffer_expr->get_byte_size();
        }
        idx++;
    }

    m_buffer_scratchpad_size = offset;
    return m_buffer_scratchpad_size > 0;
}

}  // namespace ov::snippets::lowered::pass

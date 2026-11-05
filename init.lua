--vim.g.mapleader = " "
--vim.g.maplocalleader = " "

vim.opt.number = true

vim.keymap.set("i", "nn", "<Esc>")
vim.keymap.set("v", "nn", "<Esc>")

vim.keymap.set("n", "j", "h")
vim.keymap.set("n", "k", "j")
vim.keymap.set("n", "l", "k")
vim.keymap.set("n", ";", "l")
vim.keymap.set("n", "h", "")

vim.opt.expandtab = true
vim.opt.tabstop = 4
vim.opt.softtabstop = 4
vim.opt.shiftwidth = 4
vim.opt.smartindent = true

-- Auto-open file explorer on startup
vim.api.nvim_create_autocmd("VimEnter", {
    callback = function()
        vim.cmd(":Lexplore")
    end,
})

-- Open new window (v) to the right of the current one
vim.opt.splitright = true

vim.keymap.set("n", "tt", "<cmd>vsplit<CR><cmd>terminal<CR><C-w>J")

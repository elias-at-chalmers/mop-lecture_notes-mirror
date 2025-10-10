local stringify = require("pandoc.utils").stringify

function Para(elem)
  local txt = stringify(elem)

  -- Extract filename ignoring quotes
  local filename = txt:match('{{include%s+.-([%w%._/-]+).-}}')

  if filename then
    local file = io.open(filename, "r")
    if file then
      local content = file:read("*all")
      file:close()
      return pandoc.RawBlock('html', content)
    else
      return pandoc.RawBlock('html', '<!-- Could not include file: ' .. filename .. ' -->')
    end
  end

  return elem
end
-- file: include-and-python.lua

local function read_file(path)
  local fh = io.open(path, "r")
  if not fh then return nil end
  local content = fh:read("*a")
  fh:close()
  return content
end

local function strip_outer_html(html)
  -- remove <!DOCTYPE html>
  html = html:gsub("<!DOCTYPE html>", "")
  -- remove <html> and </html>
  html = html:gsub("<html[^>]*>", ""):gsub("</html>", "")
  -- remove <head>...</head> but keep styles/scripts for inline use
  html = html:gsub("<head[^>]*>.-</head>", "")
  -- remove <body> and </body>
  html = html:gsub("<body[^>]*>", ""):gsub("</body>", "")
  return html
end

local function handle_placeholder(text)
  -- {{include filename.md}}
  local inc = text:match("^{{include%s+([^}]+)}}$")
  if inc then
    local content = read_file(inc)
    if content then
      local ext = inc:match("^.+(%..+)$")
      if ext == ".html" then
        -- strip outer HTML tags
        content = strip_outer_html(content)
        return pandoc.RawBlock("html", content)
      else
        -- parse markdown normally
        local doc = pandoc.read(content, "markdown")
        return doc.blocks
      end
    end
  end

  -- {{python script.py arg1 arg2}}
  local cmd = text:match("^{{python%s+([^}]+)}}$")
  if cmd then
    local args = {}
    for word in cmd:gmatch("%S+") do
      table.insert(args, word)
    end
    local script = table.remove(args, 1)
    local output = pandoc.pipe("python", {script, table.unpack(args)}, "")

    -- wrap in table to replace the whole Para
    return {pandoc.RawBlock("html", output)}
  end

  return nil
end

-- Handle paragraphs
function Para(el)
  local text = pandoc.utils.stringify(el)
  local replacement = handle_placeholder(text)
  if replacement then
    return replacement  -- table of blocks replaces this Para
  end
end
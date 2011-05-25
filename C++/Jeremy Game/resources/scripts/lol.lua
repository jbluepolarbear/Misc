require('coroutines')

local function SayWhen()
  i = 0
  while i < 10 do
    coroutine.yield()
	i = i + 1
  end
end

CreateCoroutine(SayWhen)

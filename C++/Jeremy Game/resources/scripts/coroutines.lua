--- coroutines
---
--- simple coroutine manager
--- Author - Jeremy Anderson


GlobalRoutines = {}

function CreateCoroutine( func )
	local co = coroutine.create( func )
	table.insert( GlobalRoutines , co )
	return co
end

function UpdateRoutines()
	for i,co in pairs (GlobalRoutines) do
		local result, err = coroutine.resume(co)
		if( result == false ) then
			GlobalRoutines[i] = nil
		end
        if err and err ~= 'cannot resume dead coroutine' then
            print(err)
        end
	end
end

function Wait( duration )
	local starttime = totalTime;
	while( totalTime - starttime < duration ) do
		coroutine.yield()
	end
end

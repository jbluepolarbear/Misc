-- from lua gems
--------------------------------------------------------------------------------------
--Event manager singleton
--------------------------------------------------------------------------------------
EventManager = class()
function EventManager:init()
    self.events = {}
    self.pendingSubscribe = {}
    self.pendingUnsubscribe = {}
end

function EventManager:FireEvent( eventName, ... )
    self.events[eventName] = self.events[eventName] or {}

    for subscriber, _ in pairs( self.events[eventName] ) do
        subscriber[eventName](subscriber, ... )
    end
end

function EventManager:Update( )
	for eventName, subscriberList in pairs( self.pendingSubscribe ) do
		for subscriber, _ in pairs( subscriberList ) do
			self:Subscribe( subscriber, eventName, true )
		end
	end
	self.pendingSubscribe = {}

	for eventName, unsubscriberList in pairs( self.pendingUnsubscribe ) do
		for unsubscriber, _ in pairs( unsubscriberList ) do
			self:Unsubscribe( unsubscriber, eventName, true )
		end
	end
	self.pendingUnsubscribe = {}
end

function EventManager:Subscribe( subscriber, eventName, bUpdating )
	if ( bUpdating ) then
		self.events[eventName] = self.events[eventName] or {}
		self.events[eventName][subscriber] = true
	else
		self.pendingSubscribe[eventName] = self.pendingSubscribe[eventName] or {}
		self.pendingSubscribe[eventName][subscriber] = true
	end
end

function EventManager:Unsubscribe( unsubscriber, eventName, bUpdating )
	if ( bUpdating ) then
		self.Events[eventName] = self.events[eventName] or {}
		self.Events[eventName][unsubscriber] = nil
	else
		self.pendingUnsubscribe[eventName] = self.pendingUnsubscribe[eventName] or {}
		self.pendingUnsubscribe[eventName][unsubscriber] = nil
	end
end

EM = EventManager()
--------------------------------------------------------------------------------------

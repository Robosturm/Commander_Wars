
var js2cppX = function(){
	return {
				_items:{},
				_id:0,
				create:function (data, id) {
					if (typeof(id) === "undefined")
					{
						this._id += 1;
						id = this._id;
					}

					this._items[id] = data;
					return id;
				},
				get:function(id){
					return this._items[id];
				},
				free:function(id){
					delete this._items[id];
				},
				get_size:function(){
					return Object.keys(this._items).length;
				}
			};
	};


var js2cpp = js2cppX();
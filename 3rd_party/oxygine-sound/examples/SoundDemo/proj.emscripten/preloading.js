function preloadFile(file, onLoadCallback){
    var r = new XMLHttpRequest();
    r.open("GET", file, true);
    r.responseType = "arraybuffer";
    r.onload = onLoadCallback;
    r.send();
}


var _preloadingCounter = 0;
function isPreloading(){
    return _preloadingCounter != 0;
}

function createFolder(path)
{
    var r = "";
    var t = path.split("/");
    for (var n=0; n < t.length - 1; ++n)
    {
        try
        {            
            r = "";
            for (var i = 0; i <= n; ++i){
                r += "/" + t[i];
            }

            FS.mkdir(r);
        } catch(e){
            var q= 0;
        }
    }
    return {"folder":r, "file":t[t.length - 1]};
}

function Preloading(id){
    return {        
    	_num:0,
        _loaded:0,
        _id:id,
        _done:function(){
            //var f = Module.cwrap('fromjs_preloaded', 'void', ['string']);
            //f(this._id);
            _preloadingCounter -=1;
        },
    	add: function(path){
            console.log("preloading " + path);
    		this._num += 1;		
            var self = this;

            
            //t += "/";
    		preloadFile(path, function(e){
                var result = new Uint8Array(e.currentTarget.response);

                createFolder(path);
                
                FS.writeFile(path, result, {encoding:'binary'});
                self._loaded += 1;
                if (self._loaded == self._num){
                    self._done.apply(self);
                }
    		});
    	},
    	start: function(){
            _preloadingCounter +=1;
    	}
    }
}

function preload_file(path){
    var p = Preloading(path);
    p.add(path);
    p.start();
}
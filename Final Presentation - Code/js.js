
    function preencherGrafico(id, valor) {
      const medidor = document.getElementById(id + "Id").querySelector('svg .circulo-progresso');
      medidor.style.strokeDasharray = [valor, 100];
      if(id == "temperatura"){
        document.getElementById(id).innerHTML = valor + "ºC";
      }else{
        document.getElementById(id).innerHTML = valor + "%";
      }
    }
    if (!!window.EventSource) {
      var source = new EventSource('/events');
    
      source.addEventListener('open', function(e) {
        console.log("Events Connected");
      }, false);
    
      source.addEventListener('error', function(e) {
        if (e.target.readyState != EventSource.OPEN) {
          console.log("Events Disconnected");
        }
      }, false);
    
      source.addEventListener('message', function(e) {
        console.log("message", e.data);
      }, false);
    
      source.addEventListener('temperatura', function(e) {
        const val = parseInt(e.data)
        console.log("temperature", val);
        preencherGrafico("temperatura", val);
      }, false);
    
      source.addEventListener('humidade', function(e) {
        console.log("humidity", e.data);
        const val = parseInt(e.data)
        preencherGrafico("humidade", val);
      }, false);
    
      source.addEventListener('rele1', function(e) {
        console.log("pressure", e.data);
        document.getElementById("buton1").innerHTML = e.data;
      }, false);
    }
    function acionaRele(id){
      let status = document.getElementById("buton"+ id).innerHTML
      if(status == "OFF"){
        window.location.href = `/on${id}`
      }else{
      window.location.href = `/off${id}`
      }
    }

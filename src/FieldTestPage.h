#pragma once
#include <Arduino.h>

// Local-only interface: no CDN, fonts, telemetry or Internet dependency.
static const char FIELD_TEST_PAGE[] PROGMEM = R"HTML(<!doctype html>
<html lang="pt-BR"><head><meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1">
<title>SolarView · Testes Modbus</title><style>
:root{color-scheme:dark;font:16px system-ui,sans-serif;background:#101820;color:#ecf2f5}*{box-sizing:border-box}body{margin:0}main{max-width:1120px;margin:auto;padding:24px}h1{font-size:1.6rem;margin-bottom:8px}h2{font-size:1.15rem}p{line-height:1.5}a{color:#7bddd2}.muted{color:#a9bbc9}.grid{display:grid;grid-template-columns:repeat(auto-fit,minmax(280px,1fr));gap:16px}.card{background:#192630;border:1px solid #304350;border-radius:12px;padding:18px;margin:16px 0}.value{font-size:1.7rem;color:#7bddd2;margin:8px 0}.warning{color:#ffcf83}.bad{color:#ffabab}.ok{color:#7bddd2}nav{display:flex;gap:18px;padding:12px 0;border-bottom:1px solid #304350}button,input,select{font:inherit;padding:10px;border-radius:7px;border:1px solid #486071;background:#0f1c25;color:#ecf2f5}button{cursor:pointer;background:#244751}button:disabled{opacity:.4;cursor:default}input[type=number],input[type=text]{min-width:0;max-width:100%;width:100%}form{display:flex;gap:8px;align-items:end;margin:12px 0;flex-wrap:wrap}label{display:block;flex:1;min-width:140px}label span{display:block;font-size:.85rem;color:#b4c6d2;margin-bottom:6px}pre{white-space:pre-wrap;overflow-wrap:anywhere;font:13px ui-monospace,monospace}#message{position:sticky;top:0;background:#213c4c;padding:12px;z-index:3;display:none;white-space:pre-wrap}small{line-height:1.5}table{border-collapse:collapse;width:100%;font-size:.9rem}td,th{text-align:left;padding:8px;border-bottom:1px solid #304350}.files{overflow:auto}details{margin:16px 0}summary{cursor:pointer}input[type=checkbox]{width:auto}button.danger{background:#553239}
</style></head><body><main>
<p class="muted">INVERTERMODBUSLIB · SOLARVIEW ESP-07</p><h1>Etapa 01 — limites de potência</h1>
<nav><a href="/">Dashboard</a><a href="/config">Sessão, comandos e arquivos</a></nav>
<div id="message" role="status"></div>
<p id="connection" class="muted">Conectando ao dispositivo…</p>
<div class="card"><strong id="sessionState">Consultando sessão</strong><p id="storage" class="muted"></p><p id="logError" class="bad"></p><small>Ao reiniciar, o dispositivo fica parado. Nenhum comando de potência é enviado automaticamente.</small></div>
<section id="dashboard"><div id="cards" class="grid"></div><div class="card"><h2>Diagnóstico</h2><pre id="health"></pre><p id="trace" class="muted"></p><p class="muted">Valor antigo permanece visível com idade e aviso de desatualização. “Pronto” indica inicialização; as respostas recentes indicam comunicação.</p></div></section>
<section id="configuration" hidden>
<div class="card"><h2>1. Preparar a sessão</h2><p>Registre o estado original dos limites no equipamento antes de alterar. O teste não lê o setpoint de volta nesta etapa.</p>
<form id="start"><label><span>Identificação do teste</span><input name="label" type="text" maxlength="48" placeholder="ID2-percentual-90" required></label><button id="startButton">Iniciar sessão e leituras</button></form>
<form id="stop"><button id="stopButton" type="submit">Encerrar sessão e liberar download</button></form>
<p class="muted">Encerrar termina a operação já aceita, salva o arquivo e pausa as leituras. Não muda os limites aplicados.</p>
<details><summary>Inversores incluídos nesta sessão</summary><div id="devices"></div><small>A seleção só pode mudar com o teste parado. Modelos, IDs e serial permanecem os da bancada; a seleção volta ao padrão após reiniciar.</small></details></div>
<div class="card"><h2>2. Comandos manuais</h2><p class="warning">90% significa 90% da potência nominal, e não 90% da geração atual. DONE/SUCCESS confirma o protocolo; confira o efeito no inversor.</p>
<form id="arm"><label><input id="enableCommands" type="checkbox"> Habilitar comandos nesta sessão</label><button>Aplicar habilitação</button></form>
<div id="commands" class="grid"></div><pre id="commandStatus"></pre>
<p class="muted">Nos SIW400G, escrever o limite também habilita a limitação. Aplicar 100% não restaura um enable anteriormente desligado. No SIW500H, W e % são caminhos diferentes: confira e reponha cada ajuste pelo mesmo caminho.</p></div>
<div class="card"><h2>3. Observações de campo</h2><form id="note"><label><span>Estado original, valor visto no equipamento ou ocorrência</span><input name="note" type="text" maxlength="100" placeholder="ID2: limite no visor=90%; potência=...; céu=..." required></label><button>Registrar no CSV</button></form><p class="muted">A hora do navegador é registrada como referência no início. O log usa também milissegundos desde o boot; não depende de Internet ou RTC.</p></div>
<div class="card"><h2>4. Arquivos de teste</h2><button id="listFiles">Atualizar arquivos</button><div id="files" class="files"></div><p class="muted">Encerre a sessão antes de baixar. Faça uma cópia no computador antes de excluir. Os arquivos não são sobrescritos automaticamente.</p>
<details><summary>Inicialização do armazenamento</summary><p class="warning">Use somente se o armazenamento ainda não foi inicializado ou se você quer apagar todos os logs. Esta ação apaga a área LittleFS configurada no firmware.</p>
<form id="format"><label><span>Digite FORMATAR</span><input name="confirm" type="text" autocomplete="off" required></label><button class="danger">Formatar área de logs</button></form></details></div>
</section></main><script>
'use strict';
const $=id=>document.getElementById(id);let state=null,built=false,pollBusy=false,sending=false,armDirty=false;const deviceDirty=new Set();
const config=location.pathname==='/config';$('configuration').hidden=!config;$('dashboard').hidden=config;
const format=(v,d=0)=>v===null||v===undefined?'pendente':Number(v).toLocaleString('pt-BR',{maximumFractionDigits:d});
function message(text,bad=false){$('message').textContent=text;$('message').className=bad?'bad':'';$('message').style.display='block';}
async function post(path,values={}){
 if(!state)throw Error('Aguarde a conexão com o dispositivo.');
 const body=new URLSearchParams({...values,token:state.token});
 const response=await fetch(path,{method:'POST',body,cache:'no-store'});const result=await response.json();
 if(!response.ok)throw Error(result.message||`HTTP ${response.status}`);message(result.message||'Concluído.');return result;
}
function connectForm(id,path,extra){$(id).addEventListener('submit',async e=>{e.preventDefault();if(sending)return;sending=true;try{const data=Object.fromEntries(new FormData(e.currentTarget));await post(path,{...data,...(extra?extra():{})});await poll();}catch(err){message(err.message,true);}finally{sending=false;}});}
function makeCards(s){
 for(const r of s.inverters){
  const card=document.createElement('article');card.className='card';card.innerHTML=`<h2></h2><p class="muted"></p><div class="value" id="power-${r.index}"></div><p id="freq-${r.index}"></p><p id="age-${r.index}"></p><pre id="info-${r.index}"></pre>`;card.querySelector('h2').textContent=r.model;card.querySelector('p').textContent=`Modbus ID ${r.id}`;$('cards').append(card);
  const cmd=document.createElement('div');cmd.className='card';cmd.innerHTML=`<h3></h3><p id="nominal-${r.index}"></p><form data-unit="percent"><label><span>Limite (%)</span><input name="percent" type="number" min="0" max="100" step="0.1" required></label><button>Aplicar %</button></form><form data-unit="watts"><label><span>Limite (W)</span><input name="watts" type="number" min="0" step="1" required></label><button>Aplicar W</button></form>`;cmd.querySelector('h3').textContent=`ID ${r.id} · ${r.model}`;
  for(const f of cmd.querySelectorAll('form'))f.addEventListener('submit',async e=>{e.preventDefault();if(sending)return;sending=true;try{const unit=f.dataset.unit;const data=Object.fromEntries(new FormData(f));await post(unit==='watts'?'/setPower':'/setPowerPercent',{...data,inv:r.index,nonce:state.nonce});await poll();}catch(err){message(err.message,true);}finally{sending=false;}});$('commands').append(cmd);
  const df=document.createElement('form');const label=document.createElement('label'),check=document.createElement('input'),button=document.createElement('button');check.type='checkbox';check.checked=r.enabled;check.id=`device-${r.index}`;check.addEventListener('change',()=>deviceDirty.add(r.index));label.append(check,document.createTextNode(` ID ${r.id} · ${r.model}`));button.textContent='Aplicar seleção';df.append(label,button);df.addEventListener('submit',async e=>{e.preventDefault();try{await post('/config/device',{inv:r.index,enable:check.checked?'1':'0'});deviceDirty.delete(r.index);await poll();}catch(err){message(err.message,true);}});$('devices').append(df);
 }built=true;
}
async function poll(){
 if(pollBusy)return;pollBusy=true;const start=performance.now();
 try{const response=await fetch('/api/status',{cache:'no-store'});if(!response.ok)throw Error(`HTTP ${response.status}`);const s=await response.json();if(state&&state.token!==s.token){armDirty=false;deviceDirty.clear();message('Dispositivo reiniciado. Confira a sessão e os inversores antes de continuar.');}state=s;if(!built)makeCards(s);
  $('connection').className='muted';$('connection').textContent=`${s.build} · ativo há ${format(s.uptime_ms/1000)} s · resposta HTTP ${Math.round(performance.now()-start)} ms`;
  $('sessionState').textContent=`${s.stopping?'ENCERRANDO':s.paused?'PARADO':'EM TESTE'} · Log: ${s.log.state} · ${s.log.name||'sem arquivo'}`;
  $('storage').textContent=`Arquivo ${format(s.log.bytes/1024,1)} KiB · livre ${format(s.log.free/1024,1)} KiB · fila ${s.log.queued} · linhas perdidas ${s.log.dropped}`;$('logError').textContent=s.log.error||'';
  const closed=s.paused&&s.quiet&&!s.stopping&&!['RECORDING','STOPPING'].includes(s.log.state);
  $('startButton').disabled=!closed;$('stopButton').disabled=closed;$('listFiles').disabled=!closed;
  for(const b of $('format').querySelectorAll('button,input'))b.disabled=!closed;
  for(const b of $('devices').querySelectorAll('button,input'))b.disabled=!closed;
  if(!armDirty)$('enableCommands').checked=s.armed;
  for(const r of s.inverters){
   if(!deviceDirty.has(r.index))$(`device-${r.index}`).checked=r.enabled;
   $(`power-${r.index}`).textContent=`${format(r.power)} W`;$(`freq-${r.index}`).textContent=`Rede: ${format(r.frequency,2)} Hz`;
   $(`age-${r.index}`).textContent=`Potência: idade ${format(r.power_age_ms===null?null:r.power_age_ms/1000,1)} s${r.stale?' · DESATUALIZADA':''}`;$(`age-${r.index}`).className=r.stale?'warning':'muted';
   $(`info-${r.index}`).textContent=`${r.enabled?'Incluído':'Excluído'} · ${r.ready?'Inicializado':'Falha de inicialização'}\nNominal: ${format(r.rated)} W (${r.rated_source})\nSerial: ${r.serial||'pendente'}\n${r.status} / ${r.modbus}\nSucesso ${r.ok} · erro ${r.errors} · timeout ${r.timeouts} · rejeição ${r.rejected}`;
   $(`nominal-${r.index}`).textContent=`Nominal ${format(r.rated)} W · ${r.rated_source}`;
   const c=$('commands').children[r.index];for(const b of c.querySelectorAll('button'))b.disabled=!s.armed||s.paused||s.stopping||!r.enabled||r.rated===null||s.pending>0||s.active.startsWith('setPower');
  }
  $('commandStatus').textContent=`Ativo: ${s.active} · pendente: ${s.pending||'nenhum'}\nÚltimo resultado: #${s.command.id} ${s.command.operation} ${s.command.value}\n${s.command.status} / ${s.command.modbus} · efeito físico: conferir no equipamento`;
  $('health').textContent=`Heap livre: ${s.heap} bytes\nMaior intervalo loop: ${s.max_loop_gap_us} µs\nMaior intervalo task Modbus: ${s.max_bus_gap_us} µs\nMaior chamada API: ${s.max_api_us} µs\nMaior escrita/flush de log: ${s.log.max_write_us} µs\nFlash física/configurada: ${s.flash_real}/${s.flash_config} bytes`;
  $('trace').textContent=s.trace||'Nenhuma transação nesta inicialização.';
 }catch(err){$('connection').textContent=`Sem atualização: ${err.message}. Valores na tela podem estar antigos.`;$('connection').className='bad';for(const b of $('commands').querySelectorAll('button'))b.disabled=true;}finally{pollBusy=false;}
}
$('enableCommands').addEventListener('change',()=>{armDirty=true;});
connectForm('start','/session/start',()=>({utc:new Date().toISOString()}));connectForm('stop','/session/stop');connectForm('arm','/session/arm',()=>{const enable=$('enableCommands').checked?'1':'0';armDirty=false;return {enable};});connectForm('note','/session/note');
$('format').addEventListener('submit',async e=>{e.preventDefault();if(!confirm('Apagar TODOS os arquivos da área LittleFS?'))return;try{await post('/logs/format',Object.fromEntries(new FormData(e.currentTarget)));await poll();await files();}catch(err){message(err.message,true);}});
async function files(){try{const response=await fetch('/api/files',{cache:'no-store'});const data=await response.json();if(!response.ok)throw Error(data.message);$('files').replaceChildren();if(!data.length){$('files').textContent='Nenhum arquivo de teste.';return;}const table=document.createElement('table');for(const f of data){const tr=document.createElement('tr'),name=document.createElement('td'),size=document.createElement('td'),actions=document.createElement('td'),link=document.createElement('a'),del=document.createElement('button');name.textContent=f.name;size.textContent=`${format(f.bytes/1024,1)} KiB`;link.textContent='Baixar CSV';link.href='/logs/download?file='+encodeURIComponent(f.name);link.download='';del.textContent='Excluir';del.className='danger';del.style.marginLeft='12px';del.onclick=async()=>{if(!confirm(`Você já salvou uma cópia? Excluir ${f.name}?`))return;try{await post('/logs/delete',{file:f.name});await files();await poll();}catch(err){message(err.message,true);}};actions.append(link,del);tr.append(name,size,actions);table.append(tr);}$('files').append(table);}catch(err){message(err.message,true);}}
$('listFiles').onclick=files;poll();setInterval(poll,2000);
</script></body></html>)HTML";

/* UI contract checks against the real embedded HTML, using a simulated API.
 * Run: NODE_PATH=<directory containing jsdom> node tests/ui/check.cjs
 * This verifies DOM/HTTP behavior, not browser rendering or ESP networking.
 */
const {JSDOM,VirtualConsole}=require('jsdom');
const fs=require('node:fs'),assert=require('node:assert/strict');
const page=fs.readFileSync('src/FieldTestPage.h','utf8').split('R"HTML(')[1].split(')HTML";')[0];
const state={build:'solarview-async-01.1',token:'boot-one',nonce:1,paused:true,stopping:false,armed:false,bus_ready:true,quiet:true,uptime_ms:1200,heap:27000,max_loop_gap_us:300,max_bus_gap_us:300,max_api_us:200,flash_real:1048576,flash_config:1048576,log:{state:'STOPPED',name:'',error:'',bytes:0,free:250000,queued:0,dropped:0,max_write_us:0},active:'NONE',pending:0,command:{id:0,operation:'NONE',value:null,status:'NONE',modbus:'NONE'},trace:null,inverters:Array.from({length:3},(_,i)=>({index:i,id:i+1,model:i?'SIW400G_T100_W0':'SIW500H_ST030_M3',enabled:true,ready:true,frequency:60,power:9000,frequency_age_ms:20,power_age_ms:20,stale:false,rated:i?100000:30000,rated_source:'inverter',serial:'TEST-'+i,ok:5,errors:0,timeouts:0,rejected:0,status:'DONE',modbus:'SUCCESS'}))};
const requests=[],errors=[];let failing=false;
const console=new VirtualConsole();console.on('jsdomError',e=>errors.push(e.message));
const dom=new JSDOM(page,{url:'http://solarview.test/config',runScripts:'dangerously',virtualConsole:console,beforeParse(w){
 w.setInterval=()=>0;w.confirm=()=>true;
 w.fetch=async(path,options={})=>{
  if(failing&&path==='/api/status')throw Error('simulated disconnect');
  let result={message:'ok'},status=200;
  if(path==='/api/status')result=structuredClone(state);
  else if(path==='/api/files')result=[{name:'/test-000001.csv',bytes:8192}];
  else if(options.method==='POST'){
   const data=Object.fromEntries(options.body);requests.push({path,data});assert.equal(data.token,state.token);
   if(path==='/session/start'){state.paused=false;state.log.state='RECORDING';state.log.name='/test-000001.csv';status=201;}
   if(path==='/session/arm')state.armed=data.enable==='1';
   if(path==='/setPowerPercent'||path==='/setPower'){assert.equal(data.nonce,String(state.nonce));state.nonce++;state.pending=state.nonce-1;status=202;}
   if(path==='/session/stop'){state.paused=true;state.log.state='STOPPED';state.pending=0;}
   if(path==='/config/device')state.inverters[Number(data.inv)].enabled=data.enable==='1';
  }
  return {ok:status<400,status,json:async()=>result};
 };
}});
const w=dom.window,$=id=>w.document.getElementById(id),tick=()=>new Promise(r=>setImmediate(r));
const submit=async(id)=>{$(id).dispatchEvent(new w.Event('submit',{bubbles:true,cancelable:true}));await tick();await tick();};
(async()=>{
 await tick();await tick();assert.equal($('commands').children.length,3);assert.equal($('configuration').hidden,false);assert.equal($('dashboard').hidden,true);
 $('start').querySelector('input').value='field-session';await submit('start');assert.equal(requests.at(-1).path,'/session/start');assert.match(requests.at(-1).data.utc,/Z$/);
 $('enableCommands').checked=true;$('enableCommands').dispatchEvent(new w.Event('change'));await w.poll();assert.equal($('enableCommands').checked,true,'poll must preserve unsent arm selection');await submit('arm');assert.equal(state.armed,true);
 const percent=$('commands').querySelector('form[data-unit=percent]');percent.querySelector('input').value='90';await w.poll();assert.equal(percent.querySelector('input').value,'90','poll must preserve typed limit');percent.dispatchEvent(new w.Event('submit',{bubbles:true,cancelable:true}));await tick();await tick();assert.equal(requests.at(-1).path,'/setPowerPercent');assert.equal(requests.at(-1).data.percent,'90');assert.equal(requests.at(-1).data.inv,'0');
 await submit('stop');await w.files();assert.match($('files').querySelector('a').href,/logs\/download\?file=%2Ftest-000001.csv/);
 state.inverters[0].enabled=false;await w.poll();assert.equal($('device-0').checked,false);state.inverters[0].enabled=true;state.token='boot-two';await w.poll();assert.equal($('device-0').checked,true,'reboot config must refresh');
 failing=true;await w.poll();assert.match($('connection').textContent,/Sem atualização/);assert.equal($('commands').querySelector('button').disabled,true);failing=false;await w.poll();assert.equal($('connection').className,'muted');
 assert.deepEqual(errors,[]);w.close();process.stdout.write('PASS: UI DOM/API checks; typing, arming, POST/nonce, file link, reboot and disconnect.\n');
})().catch(e=>{process.stderr.write(e.stack+'\n');w.close();process.exitCode=1;});

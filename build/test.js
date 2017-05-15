const spawn = require('child_process').spawn;


for(let i=1;i<=49;++i){
  tf = './testcase/test'+i+'.tig';
  const ex = spawn('./cat.out',[tf]);
  ex.stdout.on('data', (data) => {
    console.log('test '+i+' stdout:'+data);
  });
}



KEY_SIZE = 50

task :generate_key do
  bytes = []
  KEY_SIZE.times do
    bytes << format('%#x', rand(255))
  end
  
  File.write(
      File.expand_path('../config.h', __FILE__),
      "const char crypt_key[] = { #{bytes.join(', ')} };"
    )
end

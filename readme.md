

## Config

Load configuration settings from a yaml file and override them with environment variables if necessary. If value is not found it throws std::runtime_error

Precedence: (opt) load yaml settings -> (opt) load env var -> (opt) set defaults if no value found

Supported types: `int, std::string, bool, std::vector<T>`

```cpp
#include <bika/Config.h>
Config cfg{"settings.yml"};

// Load from yaml, but if env var is specified, overwrite the result
int port = cfg.get<int>("configuration.ports.web", "CUSTOM_PORT");

// Just load directly from yaml
int port = cfg.get<int>("configuration.ports.web");

// Just load directly from env var
int port = cfg.get<int>({}, "CUSTOM_PORT");

// Load a generic node
YAML::Node node = cfg.get<YAML::Node>("postgres.queries");
```
    
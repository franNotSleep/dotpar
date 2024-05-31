## dotpar - .env to TypeScript Interface Converter

`dotpar` is a command-line tool written in C that converts `.env` files into TypeScript interfaces. This utility helps with the process of defining configuration files for TypeScript projects.

## Compilation

To compile the `dotpar` program, use the provided `Makefile`. Run the following command in your terminal:

```sh
make dotpar
```

## Usage

Once compiled, you can use the `dotpar` program to convert your `.env` file into a TypeScript interface. The general usage is as follows:

```sh
./dotpar <.env path> <output.ts>
```

- `<.env path>`: Path to your input `.env` file.
- `<output.ts>`: Path to the output TypeScript file.

## Example

### .env File

Here is an example of what a `.env` file can look like:

```plaintext
#o case camel

#b enterprise
  #t int 
  PORT=123
  #b database
    #t str
    POSTGRES_DATABASE=postgres
    #t int
    POSTGRES_PORT=2032
  #q

  #b auth
    #t str
    JWT_ACCESS_SECRET=supersecret
    #t bool
    IS_JWT=true
    #t str
    JWT_REFRESH_SECRET=supersecre
  #q
#q

#b ecommerce
  #t int 
  PORT=123
  #b database
    #t str
    ECOMMERCE_DATABASE=postgres
    #t int
    ECOMMERCE_PORT=2032
  #q

  #b auth
    #t str
    ECOMMERCE_ACCESS_SECRET=supersecret
    #t str
    ECOMMERCE_REFRESH_SECRET=supersecre
  #q
#q
```

### Generated TypeScript Interface

Running `dotpar` on the above `.env` file will produce the following TypeScript interface:

```typescript
interface Configuration {
  enterprise: {
    port: number;
    database: {
      postgresDatabase: string;
      postgresPort: number;
    };
    auth: {
      jwtAccessSecret: string;
      isJwt: boolean;
      jwtRefreshSecret: string;
    };
  };
  ecommerce: {
    port: number;
    database: {
      ecommerceDatabase: string;
      ecommercePort: number;
    };
    auth: {
      ecommerceAccessSecret: string;
      ecommerceRefreshSecret: string;
    };
  };
};
```


## Contributing

Contributions are welcome! Please open an issue or submit a pull request with your improvements.

## Contact

For any questions or feedback, please reach out to [your email/contact information].

---

Enjoy using `dotpar` to streamline your TypeScript configuration management! dotpar
